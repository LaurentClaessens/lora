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

#include <boost/filesystem.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "configuration.h"
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


Configuration::Configuration(){}

Configuration::Configuration(const path starting_path,const path backup_path,const path purge_rep_path) : starting_path(starting_path),backup_path(backup_path),home_path(getenv("HOME"))
    {
        assert(  is_directory(starting_path) );
        assert(  is_directory(backup_path) );
        assert(  is_directory(purge_rep_path) );

        boost::gregorian::date now=boost::gregorian::day_clock::local_day();
        const string sd=to_simple_string(now);
        this->purge_path = purge_rep_path/sd;
        create_tree(purge_path);
        assert( is_directory(purge_path) );
    }

path Configuration::home_to_backup(const path local_path) const
    {
        string spath=local_path.string();
        string shome=home_path.string();
        string sbackup=this->backup_path.string();
        spath.replace(0,shome.size(),sbackup);
        return path(spath);
    }
path Configuration::home_to_purge(const path local_path) const
    {
        string spath=local_path.string();
        string shome=home_path.string();
        string spurge=this->purge_path.string();
        spath.replace(0,shome.size(),spurge);
        return path(spath);
    }

void Configuration::add_exclude_path(const path ex)
{
    exclude_paths.push_back(ex);
}

void Configuration::add_exclude_path(std::vector<path> vp)
{
    for (  std::vector<path>::iterator it=vp.begin();it!=vp.end();++it  )
    {
        add_exclude_path(*it);
    }
}

void Configuration::DealWithFile(const path file_path) 
    {
        const path bak_path=this->home_to_backup(file_path);
        const path purge_path=this->home_to_purge(file_path);
        if (do_we_backup(file_path,bak_path))
        {
            assert( !boost::algorithm::starts_with(bak_path,this->starting_path ) );
            pathTriple triple;
            triple.orig=file_path;
            triple.bak=bak_path;
            triple.purge=purge_path;
            FileCopyTask*  ftask= new FileCopyTask(triple);
            task_list.push_back(ftask);
        }
    }

void Configuration::DealWithRepertory(const path rep_path) {
        directory_iterator end_itr;
        for(  directory_iterator itr(rep_path); itr!=end_itr;++itr  )
        {
            path pathname=itr->path();
            if (is_directory(pathname))
            {
                path bak_rep=home_to_backup(pathname);
                if (!is_directory(bak_rep))
                {

                    RepertoryCopyTask*  dtask= new RepertoryCopyTask(pathname,bak_rep);
                    task_list.push_back(dtask);
                }
                else { DealWithRepertory(pathname); }
            }
            else if(is_regular_file(pathname)) { DealWithFile(pathname); }
            else { throw string("What the hell represents the path "+pathname.string()+" ?");
            }
        }
    }
void Configuration::MakeBackup()
    { 
        const path sp=this->starting_path;
        create_tree(home_to_backup(sp));
        DealWithRepertory(sp); 
        FinalTask*  etask= new FinalTask();
        task_list.push_back(etask);
    }
