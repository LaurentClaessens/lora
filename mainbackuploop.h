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

#ifndef __BACKUP_LOOP_H_INCLUDED__
#define __BACKUP_LOOP_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "tasks.h"
#include "mainpurgeloop.h"
#include "directoryconverter.h"

using namespace boost::filesystem;
using namespace std;

bool do_we_backup(const path orig_path,const path bak_path);         // Says if one has to perform the proposed backup.


// Constructors for 'MainBackupLoop'
// 3 arguments : starting path, backup and purge paths.
//
// * purge_modified_path is the one of the modified files; ex : /mnt/part-backup/<date>/<time>/modified
// * purge_removed_path is the one of removed files; ex : /mnt/part-backup/<date>/<time>/removed
// * purge_datetime_path is the one which contains the two others; ex : /mnt/part-backup/<date>/<time>
// * purge_path is the generic purge path; ex : /mnt/part-backup
// but the argument for the 3-parameters constructor is purge_path; ex : /mnt/part_backup

template <class Ttask_list>
class MainBackupLoop
{
    public:
        MainBackupLoop();
        MainBackupLoop(const path,const DirectoryConverter);        // The arguments are : starting, directory_converter.  The latter contains the purge, home and backup paths.

        void add_exclude_path(const path);                 // exclude the given path
        void add_exclude_path(vector<path>);         // exclude the given vector of paths 
        bool is_excluded(const path);               // says if that path is excluded from the backup
        void MakeBackup();
        MainPurgeLoop<Ttask_list> purge_loop();    // not 'const' because it
        Ttask_list get_task_list() const;            // template because maybe I want to change the type of 'task_list'
        Ttask_list* get_task_list_ptr();            // template because maybe I want to change the type of 'task_list'
        DirectoryConverter get_converter() const;
        void full_process();
    private :
        const DirectoryConverter directory_converter;
        const path starting_path;
        Ttask_list task_list;
        vector<path> excluded_paths;
        void DealWithFile(const path file_path) ;
        void DealWithRepertory(const path rep_path) ;
};


#include "mainbackuploop.cpp"

#endif  // __BACKUP_LOOP_H_INCLUDED__
