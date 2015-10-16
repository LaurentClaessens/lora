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


#ifndef __PURGE_LOOP_H_INCLUDED__
#define __PURGE_LOOP_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "mainbackuploop.h"
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool do_we_purge(const path bak_path,const path orig_path);         // if orig_path does not existe, return true because it assumes that the file has been removed since last backup cession.

class MainPurgeLoop
{
    public:
        const path starting_path;               // the starting path in home
        const path starting_backup_path;         // the starting path on which the purge process will loop
        const path backup_path;
        const path home_path;
        const path purge_modified_path;
        const path purge_removed_path;
        std::deque<GenericTask*> task_list;         // The task list is shared with the backup loop

        MainPurgeLoop();
        MainPurgeLoop(const path,const path,const path);        // The arguments are : starting_path (in home), backup_path, purge_path
                                                                // the directory starting_backup_path is then computed from starting_path

        void MakePurge();

    private :
        path backup_to_home(const path backup_path) const;
        void DealWithFile(const path file_path) ;
        void DealWithRepertory(const path rep_path) ;
};

#endif  // __PURGE_LOOP_H_INCLUDED__
