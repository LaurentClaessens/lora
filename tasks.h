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

using boost::filesystem::path;



// 'lora' has a loop over all the files/directories to be backuped and creates a list of taskes ('task_list'). That list is read by a thread which lanch the 'run' method over the elements of the task list.
// There are the following types of taskes :
// - FileCopyTask describe the operations to backup a file
// - DirectoryCopyTask describe the operations to backup a directory 
// - FinalTask is a dummy task that only says to the calling thread that we are done.
// all are derivated from GenericTask (in order to fool the type control and put more than one type in the same list).
//
// The return value of 'run' (boolean) says if one has to continue or bot with the next task. In a normal execution the only task returning 'false' is the final one created right after the main loop.



        // since "/home/foo/bar/something" can be a file as well as a directory (and the system cannot guess since the point of 'create_*_tree' is to create), 
        // we have two functions.
void create_directory_tree(const path);             // recursively creates the directory tree up to the given directory.
void create_file_tree(const path);                 // recursively creates the directory tree up to the directory of the given *file* name.

void my_copy_file(const path ,const path );      // copy but keeping the same last_write_time attribute.
void copy_tree(const path,const path);


// Example : the triple (  /home/myself/foo/bar.txt  ;  /backup/foo/bar.txt   ;   /purge/<date>/<time>/foo/bar.txt )
struct pathTriple{
    boost::filesystem::path orig;
    boost::filesystem::path bak;
    boost::filesystem::path purge;
};


class GenericTask{
    public:
        GenericTask();
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
        FileCopyTask(const pathTriple);
        bool run() const;
};

class RepertoryCopyTask : public GenericTask{
    public: 
        boost::filesystem::path orig_path;
        boost::filesystem::path bak_path;
        RepertoryCopyTask(const boost::filesystem::path orig_path,const boost::filesystem::path bak_path);
    bool run() const;
};
class FileMoveTask: public GenericTask{
    private:
        const boost::filesystem::path orig_path;
        const boost::filesystem::path destination_path;
    public:
        FileMoveTask(const path orig,const path destination);
        bool run() const;
};
class DirectoryMoveTask: public GenericTask{
    private:
        const boost::filesystem::path orig_path;
        const boost::filesystem::path destination_path;
    public:
        DirectoryMoveTask(const path orig,const path destination);
        bool run() const;
};

class FinalTask : public GenericTask{
    public:
        FinalTask() ;
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
