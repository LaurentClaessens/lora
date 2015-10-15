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

#ifndef __CONFIGURATION_H_INCLUDED__
#define __CONFIGURATION_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool do_we_backup(const path orig_path,const path bak_path);         // Says if one has to perform the proposed backup.

path purge_rep_to_purge_datetime(path);
path purge_rep_to_purge_modified(path);
path purge_rep_to_purge_removed(path);

// Constructors for 'Configuration'
// 3 arguments : starting path, backup and purge paths.
//
// * purge_modified_path is the one of the modified files; ex : /mnt/part-backup/<date>/<time>/modified
// * purge_removed_path is the one of removed files; ex : /mnt/part-backup/<date>/<time>/removed
// but the argument for the 3-parameters constructor is purge_rep_path; ex : /mnt/part_backup
class Configuration
{
    public:
        const path starting_path;
        const path backup_path;
        const path home_path;
        const path purge_modified_path;
        const path purge_removed_path;
        std::deque<GenericTask*> task_list;
        // purge_rep_path is the path to the _general_ purge repertory. Then purge_modified_path is computed and created.
        // example : purge_rep_path is  /mnt/part-backup/bakapurge/
        //           purge_modified_path is      /mnt/part-backup/bakapurge/<date>/<time>/
        // The latter is made public.

        Configuration();
        Configuration(const path,const path,const path);        // The arguments are : starting,backup,purge.

        void add_exclude_path(const path);                 // exclude the given path
        void add_exclude_path(vector<path>);         // exclude the given vector of paths 
        bool is_excluded(const path);               // says if that path is excluded from the backup
        void MakeBackup();

    private :
        vector<path> excluded_paths;
        path home_to_backup(const path local_path) const;
        path home_to_purge(const path local_path) const;
        void DealWithFile(const path file_path) ;
        void DealWithRepertory(const path rep_path) ;
};

#endif  // __CONFIGURATION_H_INCLUDED__
