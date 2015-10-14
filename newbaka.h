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
//



// header technique from http://www.cplusplus.com/forum/articles/10627/
#ifndef __NEWBAKA_H_INCLUDED__
#define __NEWBAKA_H_INCLUDED__

#include <boost/filesystem.hpp>
#include <deque>
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool run_next(std::deque<GenericTask*> &task_list);             // run the next task in 'task_list' and remove him from the list
                                                                // return false if this was the ending task and true if one has to continue.

bool do_we_backup(const path orig_path,const path bak_path);         // Says if one has to perform the proposed backup.

// Constructors for 'Configuration'
// 0 argument : read everything from the configuration file which is "newbaka.cfg".
// 1 argument : the argument is the configuration file path. 
// 3 arguments : starting path, backup and purge paths.
class Configuration
{
    public:
        const path starting_path;
        const path backup_path;
        const path home_path;
        path purge_path;
        std::deque<GenericTask*> task_list;
        // purge_rep_path is the path to the _general_ purge repertory. Then purge_path is computed and created.
        // example : purge_rep_path is  /mnt/part-backup/bakapurge/
        //           purge_path is      /mnt/part-backup/bakapurge/<date>/<time>/
        // The latter is made public.

        Configuration();
        Configuration(const path,const path,const path);        // The arguments are : starting,backup,purge.

        void add_exclude_path(const path);                 // exclude the given path
        void add_exclude_path(vector<path>);         // exclude the given vector of paths 
        void MakeBackup();

    private :
        vector<path> exclude_paths;
        path home_to_backup(const path local_path) const;
        path home_to_purge(const path local_path) const;
        void DealWithFile(const path file_path) ;
        void DealWithRepertory(const path rep_path) ;
};

// The path to be backuped is the one passed as argument. This function return that path, normalised and absolute.
path get_starting_path(int argc, char *argv[]);

// Read the file 'cfg_path' and return an object of type 'Configuration' that is ready to perform a backup.
// - starting path, backup_path, purge_path
Configuration read_configuration_file(const path cfg_path);


// This function is in a separated thread and execute the tasks in the list.
void make_the_work(  std::deque<GenericTask*> &task_list);


#endif     // __NEWBAKA_H_INCLUDED__
