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


// header technique from http://www.cplusplus.com/forum/articles/10627/
#ifndef __NEWBAKA_H_INCLUDED__
#define __NEWBAKA_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "tasks.h"
#include "mainbackuploop.h"

using namespace boost::filesystem;
using namespace std;

bool run_next(TaskList &task_list);             // run the next task in 'task_list' and remove him from the list
                                                                // return false if this was the ending task and true if one has to continue.
// The path to be backuped is the one passed as argument. This function return that path, normalised and absolute.
path get_starting_path(int argc, char *argv[]);

// Read the file 'cfg_path' and return an object of type 'MainBackupLoop' that is ready to perform a backup.
// - starting path, backup_path, purge_modified_path
MainBackupLoop read_configuration_file(const path cfg_path);


#endif     // __NEWBAKA_H_INCLUDED__
