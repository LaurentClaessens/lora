/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This is part of 'lora': you can redistribute it and/or modify
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

#include <iostream>
#include <boost/filesystem.hpp>
#include "MainLoop.h"

// MAIN LOOP ----

MainLoop::MainLoop(Configuration* config_ptr) :
    starting_path(config_ptr->getStartintPath()),
    configuration(config_prt)
{
    assert( is_directory(starting_path) );
    configuration->create_purge_directories();
    assert( configuration->are_all_paths_ok() );
}

void MainLoop::run() 
{ 
    configuration->create_purge_directories();
    loopOverDirectory(starting_path); 
}

void MainLoop::loopOverDirectory(path sub_directory)
{
    assert(is_directory(sub_directory));

    directory_iterator end_itr;
    for(  directory_iterator itr(backup_path); itr!=end_itr;++itr  )
    {
        path pathname=itr->path();
        if (is_directory( pathname  )) { DealWithDirectory(pathname); }
        else if (is_regular_file(pathname)) { DealWithFile(pathname); }
        else
        {
            throw string("**  What the f*ck is "+pathname.string()+" ??? ");
        }
    }
}

// MAIN BACKUP LOOP ----

bool MainBackupLoop::is_excluded(path dirname)
{
    return configuration->is_excluded(dirname); 
}



MainBackupLoop::DealWithDirectory(path rep_path)
{
    if (!is_excluded(rep_path))
    {
        path bak_rep=configuration->home_to_backup(pathname);
        if (!is_directory(bak_rep)) { create_directory_tree(bak_rep); }
        loopOverDirectory(pathname); 
    }
}

MainBackupLoop::DealWithFile(path file_path)
{
    assert( is_regular_file(file_path) );
    const path bak_path=configuration->home_to_backup(file_path);
    const path purge_modified_path=configuration->home_to_modified_purge(file_path);
    if (do_we_backup(file_path,bak_path))
    {
        // This assert checks that we will not write in the home directory.
        assert( !boost::algorithm::starts_with(bak_path,configuration->home_path ) );

        pathTriple triple;
        triple.orig=file_path;
        triple.bak=bak_path;
        triple.purge=purge_modified_path;
        FileCopyTask* ftask= new FileCopyTask(triple);
        configuration->add_task(ftask);
    }
}

// MAIN PURGE LOOP ----

void MainPurgeLoop::run() 
{ 
    MainLoop()::run();
    FinalTask* etask= new FinalTask();
    configuration->add_task(etask);
}

void MainPurgeLoop::DealWithFile(const path pathname)
{
    if(!is_regular_file(configuration->backup_to_home(pathname)   ))
    {
        FileMoveTask*  mtask= new FileMoveTask(pathname, configuration->backup_to_removed_purge(pathname)  );
        configuration->add_task(mtask);
    }
}

void MainPurgeLoop::DealWithDirectory(const path backup_path)
{
    if (!is_directory( configuration->backup_to_home(backup_path)  ))
    {
        DirectoryMoveTask*  dtask= new DirectoryMoveTask(backup_path, configuration->backup_to_removed_purge(backup_path)  );
        configuration->add_task(dtask);
    }
    loopOverDirectory(backup_path);
}
