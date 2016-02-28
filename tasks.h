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
//

#ifndef __INCLUDE_TASKS_H__
#define __INCLUDE_TASKS_H__


#include <deque>
#include <boost/filesystem.hpp>
#include "Configuration.h"

using boost::filesystem::path;

/*
 
ABOUT TASKS
 
'lora' has a loop over all the files/directories to be backuped and creates a list of taskes ('task_list'). That list is read by a thread which lanch the 'run' method over the elements of the task list.
There are the following types of tasks :
- FileCopyTask describe the operations to backup a file
- DirectoryCopyTask describe the operations to backup a directory 
- FinalTask is a dummy task that only says to the calling thread that we are done.
all are derived from GenericTask (in order to fool the type control and put more than one type in the same list).

// 'lora' has a loop over all the files/directories to be backuped and creates a list of taskes ('task_list'). That list is read by a thread which lanch the 'run' method over the elements of the task list.
// There are the following types of taskes :
// - FileCopyTask describe the operations to backup a file
// - DirectoryCopyTask describe the operations to backup a directory 
// - FinalTask is a dummy task that only says to the calling thread that we are done.
// all are derivated from GenericTask (in order to fool the type control and put more than one type in the same list).
//
// The return value of 'run' (boolean) says if one has to continue or bot with the next task. In a normal execution the only task returning 'false' is the final one created right after the main loop.

ABOUT UTILITIES

The class Utilities is intended to be used as anonymous.  The aim is to make the 'config_ptr' available for logging purposes. See the comment in 'Configuration.h' for some rationales.

The constructor with no argument initialize 'config_ptr' to the nul pointer. You should not use that constructor since any logging inside the function will result in a crash.
Of course, if something has to be logged, it means that something strange happened and you may _prefer_ to have a crash.

Since "/home/foo/bar/something" can be a file as well as a directory (and the system cannot guess since the point of 'create_*_tree' is to create), we have two functions.
class Utilities

   Why is Utilities not static ?

As explained in 'Configuration.h', the variable 'config_ptr' is an attempt to avoid a global variable. So we pass it as a parameter to more or less all objects.
   Here we can expect 'create_directory_tree' to be 'static', but it is not because :
- it needs 'config_ptr' for logging purpose
- it needs 'config_ptr' for 'assert' purpose. I want to be able to check if we are writing in $HOME at every moment. In other words, a future release of Lora is likely to add something like 
        assert( file_path 'not inside' config_ptr->getHomePath() )
  in the create_tree functions.      

     Possible improvement :

It could be clearer to add "Utilities" inside Configuration, and call it with
        config_ptr->create_directory_tree(...)
instead of
        Utilities(config_ptr)->create_directory_tree(...)
 
Because the behaviour of these function *is expected* to depend on the configuration file. That could be coherent with the deprecation of the zero-parameter constructor.

//*/
class Utilities
{
    private : 
        const Configuration* config_ptr;
        // since "/home/foo/bar/something" can be a file as well as a directory (and the system cannot guess since the point of 'create_*_tree' is to create), 
        // we have two functions.
    public:
        Utilities(const Configuration*);

        // This constructor is intended to be deprecated in a future version of Lora.
        Utilities();

        void create_directory_tree(const path);             // recursively creates the directory tree up to the given directory.
        void create_file_tree(const path);                 // recursively creates the directory tree up to the directory of the given *file* name.

        void my_copy_file(const path ,const path );      // copy but keeping the same last_write_time attribute.
        void copy_tree(const path,const path);
};


// Example : the triple (  /home/myself/foo/bar.txt  ;  /backup/foo/bar.txt   ;   /purge/<date>/<time>/foo/bar.txt )
struct pathTriple{
    boost::filesystem::path orig;
    boost::filesystem::path bak;
    boost::filesystem::path purge;
};

class GenericTask{
    protected:
        const Configuration* config_ptr;
    public:
        GenericTask(const Configuration* config);
        virtual ~GenericTask();
        virtual bool run() const =0;
};

// FileCopyTask do
// - move the backup file (if exists) to the purge repertory
// - copy the home file to the backup repertory.
// The main loop (especially DealWith...) determines the paths : this run() is stupid.
// The last_write_time attribute of the copied file is the one of the original file.
class FileCopyTask : public GenericTask{
    private: 
        boost::filesystem::path orig_path;
        boost::filesystem::path bak_path;
        boost::filesystem::path purge_modified_path;
    public:
        FileCopyTask(const pathTriple,const Configuration* config);
        bool run() const;
};

class RepertoryCopyTask : public GenericTask{
    public: 
        boost::filesystem::path orig_path;
        boost::filesystem::path bak_path;
        RepertoryCopyTask(const boost::filesystem::path orig_path,const boost::filesystem::path bak_path,const Configuration* config);
    bool run() const;
};
class FileMoveTask: public GenericTask{
    private:
        const boost::filesystem::path orig_path;
        const boost::filesystem::path destination_path;
    public:
        FileMoveTask(const path orig,const path destination,const Configuration* config);
        bool run() const;
};
class DirectoryMoveTask: public GenericTask{
    private:
        const boost::filesystem::path orig_path;
        const boost::filesystem::path destination_path;
    public:
        DirectoryMoveTask(const path orig,const path destination,const Configuration* config);
        bool run() const;
};

class FinalTask : public GenericTask{
    public:
        FinalTask(Configuration* config);
        bool run() const;
};

class TaskList{
    private:
        std::deque<GenericTask*> queue;
    public:
        void push_back(GenericTask*);
        GenericTask* front();
        void pop_front();
        int size() const;
};


#endif      //__INCLUDE_TASKS_H__
