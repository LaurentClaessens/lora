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

template <class Ttask_list> MainPurgeLoop<Ttask_list>::MainPurgeLoop(const path starting_path,const path backup_path,const path purge_path,const DirectoryConverter directory_converter) : directory_converter(directory_converter)
    {
        directory_converter.create_purge_directories();
        assert( directory_converter.are_all_paths_ok() );
    }

template <class Ttask_list> void MainPurgeLoop<Ttask_list>::MakePurge()
{
    DealWithDirectory(directory_converter.backup_path);
}

template <class Ttask_list>void MainPurgeLoop<Ttask_list>::DealWithDirectory(const path backup_path)
{
    assert(is_directory(backup_path));
    const path home_rep_path=directory_converter.backup_to_home(backup_path);
    std::cout<<"(purge) Le répertoire "<<backup_path<<"correspond à "<<home_rep_path<<std::endl;
    directory_iterator end_itr;
    for(  directory_iterator itr(backup_path); itr!=end_itr;++itr  )
    {
        path truc=directory_converter.backup_to_home(itr->path());
        std::cout<<"(purge) Le truc "<<truc<<"correspond à "<<home_rep_path<<std::endl;
    }
}

template <typename Ttask_list> Ttask_list MainPurgeLoop<Ttask_list>::get_task_list()
{
    return task_list;
}
