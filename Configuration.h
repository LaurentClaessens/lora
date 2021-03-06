/*
Copyright 2015-2016 Laurent Claessens
contact : laurent@claessens-donadello.eu

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


#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
#include <boost/filesystem.hpp>
#include "DirectoryConverter.h"
#include "HashTable.h"
#include "Logging.h"

using std::string;

class TaskList;
class GenericTask;

// The aim of this class is to answer the questions which depend on the
// configuration (paths manipulation, excluded files, task_list, ...)
// This contains a direct sum of
// - directory converter,
// - task list,
// - excluded path,
// - a logging system
// and some functions to access all that.
//
// Let be clear once for all : 'config_ptr' is more or less a global variable : it has to be available almost everywhere, at least for logging purpose. Since I don't want a global variable, it is passed as argument to almost every objects in Lora.
class Configuration
{
    private:
        const path starting_backup_path;
        const DirectoryConverter* const converter_ptr;
        TaskList* const task_list_ptr;
        std::vector<path> excluded_paths;
        Logging*  logging;
    public:
        Configuration(const path,const DirectoryConverter* const,TaskList* const);
        path getStartingBackupPath() const;
        path getHomePath() const;
        path getBackupPath() const;
        path getPurgePath() const;

        // excluding a non-existing path throws an exception. 
        void add_exclude_path(const path, const bool verbose=false);
        void add_exclude_path(const std::vector<path>, const bool verbose=false); 

        void add_task(GenericTask*);
        bool is_excluded(const path) const;       
        TaskList*  getTaskList() const;

        path backup_to_removed_purge(const path) const;
        path backup_to_home(const path) const;
        path home_to_backup(const path) const;
        path home_to_modified_purge(const path) const;            

        bool do_we_backup(const path orig_path,const path bak_path) const;
        void create_purge_directories() const;
        bool are_all_paths_ok() const;             

        void setLog(Logging*);
        void setLogFile(string);
        void writeLog(string) const;

        void processEvents();
};

path get_starting_backup_path(int argc, char *argv[]);

// Read the given configuration file and return
// an HashTable whose keys are the "before the = sign" and values are vectors
// of string "after the = signe".
// example.
// if the configuration file contains
// foo=bla
// bar=blo
// foo=bli
// the returned hash table will satisfy
// ht["foo"]=(bla,bli)
// ht["bar"]=(blo)
HashTable<std::string,std::vector<std::string>> read_configuration_file(const path cfg_path);
Configuration* arguments_to_configuration(const int argc=0,char* argv[]=0, const bool verbose=true);
Configuration* configuration_file_to_configuration(const path cfg_path,const path starting_backup_path="", const bool verbose=true);


// The following returns the last found value (in the file) of the required property.
std::string read_configuration_file(const path cfg_path,const std::string searched_property);

path get_starting_backup_path(int,char*);

#endif     //__CONFIGURATION_H__
