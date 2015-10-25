/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*/

#include <ctime>            // get the current time
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "mainbackuploop.h"
#include "mainpurgeloop.h"
#include "tasks.h"

using namespace boost::filesystem;


bool do_we_backup(path orig_path,path bak_path)
{
    assert(is_regular_file(orig_path));
    if (!is_regular_file(bak_path)){return true;}
    uintmax_t s_orig=file_size(orig_path);
    uintmax_t s_bak=file_size(bak_path);
    if (s_orig!=s_bak){return true;}
    time_t t_ori=last_write_time(orig_path);
    time_t t_bak=last_write_time(bak_path);
    if (t_ori>t_bak){return true;}
    if (t_ori<t_bak){throw string("The last_write_date of this file is f*cked up !"+orig_path.string()+" "+bak_path.string()+" ?");}
    return false;
}

// MainBackupLoop::MainBackupLoop(){}        //One cannot use that constructor because it causes a non initialized const pointer.

// the main backup loop is not intended to be constructed "by hand".
// It is constructed and returned by the 'read_configuration_file' function. This is because it has to posses a DirectoryConverter which has to be constructed separately.
MainBackupLoop::MainBackupLoop(const path starting_path,const DirectoryConverter* const dc_ptr,TaskList* const tl_ptr) : 
    starting_path(starting_path),
    task_list_ptr(tl_ptr),
    converter_ptr(dc_ptr)
{
    assert( is_directory(starting_path) );
    assert(converter_ptr->are_all_paths_ok());
}

void MainBackupLoop::add_exclude_path(const path ex)
{
    excluded_paths.push_back(ex);
}

void MainBackupLoop::add_exclude_path(std::vector<path> vp)
{
    for (  std::vector<path>::iterator it=vp.begin();it!=vp.end();++it  )
    {
        add_exclude_path(*it);
    }
}

void MainBackupLoop::DealWithFile(const path file_path) 
    {
        assert( is_regular_file(file_path) );
        const path bak_path=get_converter_ptr()->home_to_backup(file_path);
        const path purge_modified_path=get_converter_ptr()->home_to_modified_purge(file_path);
        if (do_we_backup(file_path,bak_path))
        {
            assert( !boost::algorithm::starts_with(bak_path,starting_path ) );
            pathTriple triple;
            triple.orig=file_path;
            triple.bak=bak_path;
            triple.purge=purge_modified_path;
            FileCopyTask*  ftask= new FileCopyTask(triple);
            get_task_list_ptr()->push_back(ftask);
        }
    }

void MainBackupLoop::DealWithRepertory(const path rep_path) {
        if (!is_excluded(rep_path))
        {
            directory_iterator end_itr;
            for(  directory_iterator itr(rep_path); itr!=end_itr;++itr  )
            {
                path pathname=itr->path();
                if (is_directory(pathname))
                {
                    path bak_rep=get_converter_ptr()->home_to_backup(pathname);
                    if (!is_directory(bak_rep))
                    {
                        RepertoryCopyTask*  dtask= new RepertoryCopyTask(pathname,bak_rep);
                        get_task_list_ptr()->push_back(dtask);
                    }
                    else { DealWithRepertory(pathname); }
                }
                else if(is_regular_file(pathname)) { DealWithFile(pathname); }
                else { throw string("What the hell represents the path "+pathname.string()+" ?");
                }
            }
        }
    }

bool MainBackupLoop::is_excluded(const path pathname) const
{
    for (  std::vector<path>::iterator iter=excluded_paths.begin();iter!=excluded_paths.end();++iter  )
    {
        if (*iter==pathname) {return true;}
    }
    return false;
} 

void MainBackupLoop::MakeBackup()
{ 
    create_directory_tree(get_converter_ptr()->home_to_backup(starting_path));
    DealWithRepertory(starting_path); 
}

MainPurgeLoop MainBackupLoop::purge_loop() { return MainPurgeLoop( converter_ptr,task_list_ptr ); }

TaskList* const MainBackupLoop::get_task_list_ptr() const { return task_list_ptr; }

const DirectoryConverter* const MainBackupLoop::get_converter_ptr() const { return converter_ptr; }
