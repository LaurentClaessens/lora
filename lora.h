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


// header technique from http://www.cplusplus.com/forum/articles/10627/
#ifndef __LORA_H_INCLUDED__
#define __LORA_H_INCLUDED__

#include <boost/filesystem.hpp>
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool run_next(TaskList &task_list);
path get_starting_path(int argc, char *argv[]);

MainBackupLoop read_configuration_file(const path cfg_path,const path starting_path);


#endif     // __LORA_H_INCLUDED__
