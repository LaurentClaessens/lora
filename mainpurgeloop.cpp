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

template <class Ttask_list> MainPurgeLoop<Ttask_list>::MainPurgeLoop(const DirectoryConverter directory_converter) : 
    directory_converter(directory_converter)
    {
        directory_converter.create_purge_directories();
        assert( directory_converter.are_all_paths_ok() );
    }

template <class Ttask_list> void MainPurgeLoop<Ttask_list>::MakePurge()
{
    DealWithDirectory(directory_converter.get_backup_path());
}

template <class Ttask_list>void MainPurgeLoop<Ttask_list>::DealWithFile(const path pathname)
{
    if(!is_regular_file(   directory_converter.backup_to_home(pathname)   ))
    {
        cout<<"En voila un fichier qu'il faudra virer du backup : "<<pathname<<endl;
        cout<<"On va déplacer "<<pathname<<" vers "<<directory_converter.backup_to_removed_purge(pathname)<<endl;
        FileMoveTask*  mtask= new FileMoveTask(pathname, directory_converter.backup_to_removed_purge(pathname)  );
        task_list.push_back(mtask);
    }
}

template <class Ttask_list>void MainPurgeLoop<Ttask_list>::DealWithDirectory(const path backup_path)
{
    assert(is_directory(backup_path));
    path corresponding_home=directory_converter.backup_to_home(backup_path);

    if (!is_directory( directory_converter.backup_to_home(backup_path)  ))
    {
        cout<<"Il faudra supprimer du backup le répertoire :"<<backup_path<<endl;
        DirectoryMoveTask*  dtask= new DirectoryMoveTask(backup_path, directory_converter.backup_to_removed_purge(backup_path)  );
        task_list.push_back(dtask);
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

template <typename Ttask_list> Ttask_list MainPurgeLoop<Ttask_list>::get_task_list()
{
    return task_list;
}
