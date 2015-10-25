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

#ifndef __PURGE_LOOP_H_INCLUDED__
#define __PURGE_LOOP_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "mainbackuploop.h"
#include "directoryconverter.h"
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool do_we_purge(const path bak_path,const path orig_path);         // if orig_path does not existe, return true because it assumes that the file has been removed since last backup cession.


// task_list and directory_converter are privately pointers because they are shared with the backup loop.
// This is not absolutely necessary for the directory converter while it is necessary for the task list : the (hope not much infinite) loop 'make_the_work' have to run the backup tasks as well as the purge ones.

class MainPurgeLoop
{
    public:
        MainPurgeLoop();
        MainPurgeLoop(DirectoryConverter* const dc_ptr,TaskList* const tl_ptr);

        TaskList* const get_task_list_ptr() const;      
        const DirectoryConverter* const get_converter_ptr() const;
        void MakePurge();
    private :
        const DirectoryConverter* const converter_ptr;
        TaskList* const task_list_ptr;                  // be careful : the pointer is constant. The task list is obviously alway changing.
        void DealWithFile(const path file_path) ;
        void DealWithDirectory(const path rep_path) ;
};

#include "mainpurgeloop.cpp"

#endif  // __PURGE_LOOP_H_INCLUDED__
