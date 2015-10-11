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

bool run_next(std::deque<GenericTask*> &task_list);           // run the next task in 'task_list' and remove him from the list

bool create_tree(const path);             // recursively creates the directory tree up to the given directory.
void my_copy_file(const path ,const path );     // copy 'from_path' to 'to_path' keeping the 'last_write_time' attribute.
void copy_tree(const path,const path);
bool create_tree(const path rep_path);
void my_copy_file(const path from_path,const path to_path);      // copy but keeping the same last_write_time attribute.
void copy_tree(const path orig_path,const path bak_path);

                                                        // return false if this was the ending task and true if one has to continue.
bool do_we_backup(const path orig_path,const path bak_path);         // Says if one has to perform the proposed backup.


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
    Configuration(const path starting_path,const path backup_path,const path purge_rep_path);

    path home_to_backup(path local_path) const;
    path home_to_purge(path local_path) const;
    void DealWithFile(path file_path) const;
    void DealWithRepertory(path rep_path) ;
    void MakeBackup();
};

path get_starting_path(int argc, char *argv[])
{
    path starting_path;
    path full_path;
    if (argc != 2)
    {
        throw std::string("You have to pass a repertory name or path.");
    }
    else
    {
        starting_path=path(argv[1]);
        if (starting_path.is_relative()){
            full_path=absolute(starting_path);
        }
        else {
            full_path=starting_path;
        }
    }
    full_path=canonical(full_path);
    std::cout<<"We are going to backup the repertory "<<full_path<<endl;
    return full_path;
}

// This function is in a separated thread and execute the tasks in the list.
void make_the_work(  std::deque<GenericTask*> &task_list)
{
    bool still=true;
    while (still)
    {
        if (task_list.size() != 0)
        {
            try{
             still=run_next(task_list);
            }
            catch (std::string err) { std::cout<<std::string("I got a bad news : ")<<err<<endl; }
        }
    }
}



#endif     // __NEWBAKA_H_INCLUDED__
