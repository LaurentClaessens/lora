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
#include "mainbackuploop.h"
#include "mainpurgeloop.h"
#include "tasks.h"


MainPurgeLoop::MainPurgeLoop(){}

MainPurgeLoop::MainPurgeLoop(const DirectoryConverter &directory_converter, TaskList &task_list) 
    {
        ptr_converter=&directory_converter;
        ptr_task_list=&task_list;
        directory_converter.create_purge_directories();
        assert( directory_converter.are_all_paths_ok() );
    }

DirectoryConverter MainPurgeLoop::get_converter() const
{
    return *ptr_converter;
}
TaskList MainPurgeLoop::get_task_list() const { return *ptr_task_list; }
TaskList* MainPurgeLoop::get_task_list_ptr() const { return ptr_task_list; }

void MainPurgeLoop::MakePurge()
{
    DealWithDirectory(get_converter().get_backup_path());
    FinalTask*  etask= new FinalTask();
    get_task_list_ptr()->push_back(etask);
}

void MainPurgeLoop::DealWithFile(const path pathname)
{
    if(!is_regular_file(   get_converter().backup_to_home(pathname)   ))
    {
        FileMoveTask*  mtask= new FileMoveTask(pathname, get_converter().backup_to_removed_purge(pathname)  );
        get_task_list_ptr()->push_back(mtask);
    }
}

void MainPurgeLoop::DealWithDirectory(const path backup_path)
{
    assert(is_directory(backup_path));
    path corresponding_home=get_converter().backup_to_home(backup_path);

    if (!is_directory( get_converter().backup_to_home(backup_path)  ))
    {
        DirectoryMoveTask*  dtask= new DirectoryMoveTask(backup_path, get_converter().backup_to_removed_purge(backup_path)  );
        get_task_list_ptr()->push_back(dtask);
    }
    else 
    {
        directory_iterator end_itr;
        for(  directory_iterator itr(backup_path); itr!=end_itr;++itr  )
        {
            path pathname=itr->path();
            if (is_directory( pathname  )){
                DealWithDirectory(pathname);
            }
            else if (is_regular_file(pathname))
            {
                DealWithFile(pathname);
            }
            else {
                throw string("**  What the f*ck is "+pathname.string()+" ??? ");
            }
        }

    }
}
