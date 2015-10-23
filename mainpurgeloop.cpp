/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This is part of 'newbaka': you can redistribute it and/or modify
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


template <class Ttask_list> MainPurgeLoop<Ttask_list>::MainPurgeLoop(){}

template <class Ttask_list> MainPurgeLoop<Ttask_list>::MainPurgeLoop(const DirectoryConverter &directory_converter, Ttask_list &task_list) 
    {
        ptr_converter=&directory_converter;
        ptr_task_list=&task_list;
        directory_converter.create_purge_directories();
        assert( directory_converter.are_all_paths_ok() );
    }

template <class Ttask_list> DirectoryConverter MainPurgeLoop<Ttask_list>::get_converter() const
{
    return *ptr_converter;
}
template <class Ttask_list> Ttask_list MainPurgeLoop<Ttask_list>::get_task_list() const { return *ptr_task_list; }
template <class Ttask_list> Ttask_list* MainPurgeLoop<Ttask_list>::get_task_list_ptr() const { return ptr_task_list; }

template <class Ttask_list> void MainPurgeLoop<Ttask_list>::MakePurge()
{
    cout<<"Purge: adresse de ma liste "<<ptr_task_list<<endl;
    DealWithDirectory(get_converter().get_backup_path());
    cout<<"Je crée et place la tâche finale -- "<<get_task_list().size()<<endl;
    FinalTask*  etask= new FinalTask();
    get_task_list().push_back(etask);
}

template <class Ttask_list>void MainPurgeLoop<Ttask_list>::DealWithFile(const path pathname)
{
    if(!is_regular_file(   get_converter().backup_to_home(pathname)   ))
    {
        FileMoveTask*  mtask= new FileMoveTask(pathname, get_converter().backup_to_removed_purge(pathname)  );
        get_task_list().push_back(mtask);
    }
}

template <class Ttask_list>void MainPurgeLoop<Ttask_list>::DealWithDirectory(const path backup_path)
{
    assert(is_directory(backup_path));
    path corresponding_home=get_converter().backup_to_home(backup_path);

    if (!is_directory( get_converter().backup_to_home(backup_path)  ))
    {
        DirectoryMoveTask*  dtask= new DirectoryMoveTask(backup_path, get_converter().backup_to_removed_purge(backup_path)  );
        cout<<"je crée une tâche pour"<<backup_path<<endl;
        cout<<"taille : "<<get_task_list().size();
        get_task_list().push_back(dtask);
        cout<<"taille : "<<get_task_list().size();
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
