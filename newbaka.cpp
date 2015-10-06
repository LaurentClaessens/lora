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
//
//
// g++  newbaka.cpp  -o newbaka /usr/lib/i386-linux-gnu/libboost_filesystem.a   /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system
//

// TODO : should task_list be a shared pointer ? Could that avoid my 'still' boolean in make_the_work ?

#include <iostream>
#include <string>
#include <deque>
#include <pwd.h>                            // To get the user's home repertory.
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace boost::filesystem;
using namespace std;



// My prototypes
void DealWithRepertory(path);
void DealWithFile(path);


bool create_tree(path);      // recursively creates the directory tree up to 'rep_path'.
void my_copy_file(path ,path );  // copy 'from_path' to 'to_path' keeping the 'last_write_time' attribute.
                                // the destination file cannot exist (in case of home->backup, the file has to be moved to the purge first).

// Example : the triple (  /home/myself/foo/bar.txt  ;  /backup/foo/bar.txt   ;   /purge/<date>/<time>/foo/bar.txt )
struct PathTriple{
    path orig;
    path bak;
    path purge;
};

bool create_tree(path rep_path)
{
    path parent_path=rep_path.parent_path();
    if (is_directory( parent_path ) ) {}
    else { create_tree(parent_path); } 
    create_directory(rep_path); 
}

void my_copy_file(path from_path,path to_path)
{
    assert( is_regular_file(from_path) );
    assert( !is_regular_file(to_path) );

    time_t t_ori=last_write_time(from_path);
    copy_file(from_path,to_path);
    last_write_time( to_path,t_ori );

    assert( last_write_time(from_path)==last_write_time(to_path) );
}

// The derived class run() member have to return a boolean saying either one has to continue the work after or not.
// returning true make the work continue (either performing next task either waiting for a new task to arrive).
// returning false exits the working thread and closes the execution of the program.
class GenericTask{
    public:
        GenericTask(){ }
        virtual bool run(){ throw string("You tried to run a GenericTask"); }
};

// FileCopyTask do
// - move the backup file (if exists) to the purge repertory
// - copy the home file to the backup repertory.
// The main loop (especially DealWith...) determines the paths : this run() is stupid.
// The last_write_time attribute of the copied file is the one of the original file.
class FileCopyTask : public GenericTask{
    private: 
        path orig_path;
        path bak_path;
        path purge_path;
    FileCopyTask(pathTriple triple):GenericTask()
    {
        this->orig_path=triple.home;
        this->bak_path=triple.bak;
        this->purge_path=triple.purge;
    }
    bool run(){
        assert(is_regular_file(orig_path));


        cout<<"(file) Copy  "<<this->orig_path<<endl;
        cout<<"--->  "<<this->bak_path<<"...";

        if (is_regular_file(bak_path))
        {
            rename( bak_path,purge_path );
        }
        my_copy_file(  orig_path,bak_path  );

        assert( is_regular_file(orig_path) );
        assert( is_regular_file(bak_path) );
        assert( is_regular_file(purge_path) );
        return true;
    }
};

void copy_tree(path orig_path,path bak_path)
{
    cout<<"(rep) Copy "<<orig_path<<" --> "<<bak_path;
    create_directory(bak_path);
    directory_iterator end_itr;
    for(  directory_iterator itr(orig_path); itr!=end_itr;++itr  )
    {
        path pathname=itr->path();
        path bak_sub=bak_path/pathname.filename();
        if (is_directory(pathname))
        {  
            create_directory(bak_sub);
            copy_tree(pathname,bak_sub);
        }
        else if (is_regular_file(pathname))
        {
            copy_file(pathname,bak_sub,copy_option::overwrite_if_exists);
        }
    }
    cout<<"done (rep)"<<endl;
}

class RepertoryCopyTask : public GenericTask{
    public: 
        path orig_path;
        path bak_path;
        RepertoryCopyTask(path orig_path,path bak_path):GenericTask()
    {
        this->orig_path=orig_path;
        this->bak_path=bak_path;
    }
    bool run()
    {
        cout<<"Copy  "<<this->orig_path<<endl;
        cout<<"--->  "<<this->bak_path<<endl;
        copy_tree(orig_path,bak_path);
        return true;
    }
};

class EndingTask : public GenericTask{
    public:
        bool ending_task;
    EndingTask(){ }
    bool run()
    {
        return false;
    }
};


bool run_next(deque<GenericTask*> &task_list){
    // run the next task in 'task_list' and remove him from the list
    // return false if this was the ending task and true if one has to continue.
    bool ret;
    ret=task_list.front()->run();       // equivalent to   (*task_list.front()).run()
    task_list.pop_front();
    cout<<task_list.size()<<" tasks remaining"<<endl;
    return ret;
}

// Says if one has to perform the proposed backup.
bool do_we_backup(path orig_path,path bak_path)
{
    if (!is_regular_file(bak_path)){return true;}
    uintmax_t s_orig=file_size(orig_path);
    uintmax_t s_bak=file_size(bak_path);
    if (s_orig!=s_bak){return true;}
    time_t t_ori=last_write_time(orig_path);
    time_t t_bak=last_write_time(bak_path);
    if (t_ori>t_bak){return true;}
    //TODO : the copied file has last_write_time set to backup time. One has to set it to the one of the original file.
    //if (t_ori<t_bak){throw string("The last_write_date of this file is f*cked up !"+orig_path.string()+" "+bak_path.string()+" ?");}
    return false;
}

class Configuration{
    public:
        path starting_path;
        path backup_path;
        path home_path;
        path purge_path;
        deque<GenericTask*> task_list;
        // purge_rep_path is the path to the _general_ purge repertory. Then purge_path is computed and created.
        // example : purge_rep_path is  /mnt/part-backup/bakapurge/
        //           purge_path is      /mnt/part-backup/bakapurge/<date>/<time>/
        // The latter is made public.
    Configuration(path starting_path,path backup_path, path purge_rep_path)
    {
        this->starting_path=starting_path;
        this->backup_path=backup_path;
        this->home_path=getenv("HOME");
    }
    path home_to_backup(path local_path)
    {
        string spath=local_path.string();
        string shome=home_path.string();
        string sbackup=this->backup_path.string();
        spath.replace(0,shome.size(),sbackup);
        return path(spath);
    }

    void DealWithFile(path file_path){
        path bak_path=this->home_to_backup(file_path);
        if (do_we_backup(file_path,bak_path))
        {
            assert( !starts_with(bak_path,this->starting_path ) );
            FileCopyTask*  ftask= new FileCopyTask(file_path,bak_path);
            task_list.push_back(ftask);
        }
    }

    //void DealWithRepertory(string rep_name){ DealWithRepertory( path(rep_name) ); }
    void DealWithRepertory(path rep_path) {
    
        directory_iterator end_itr;
        for(  directory_iterator itr(rep_path); itr!=end_itr;++itr  )
        {
            path pathname=itr->path();
            if (is_directory(pathname))
            {
                path bak_rep=home_to_backup(pathname);
                if (!is_directory(bak_rep))
                {

                    RepertoryCopyTask*  dtask= new RepertoryCopyTask(pathname,bak_rep);
                    task_list.push_back(dtask);
                }
                else { DealWithRepertory(pathname); }
            }
            else if(is_regular_file(pathname)) { DealWithFile(pathname); }
            else { throw string("What the hell represents the path "+pathname.string()+" ?");
            }
        }
    }
    void MakeBackup()
    { 
        path sp=this->starting_path;
        create_tree(home_to_backup(sp));
        DealWithRepertory(sp); 
        EndingTask*  etask= new EndingTask();
        task_list.push_back(etask);
    }
};

path get_starting_path(int argc, char *argv[])
{
    path starting_path;
    path full_path;
    if (argc != 2)
    {
        throw string("You have to pass a repertory name or path.");
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
    cout<<"We are going to backup the repertory "<<full_path<<endl;
    return full_path;
}

// This function is in a separated thread and execute the tasks in the list.
void make_the_work(  deque<GenericTask*> &task_list)
{
    bool still=true;
    while (still)
    {
        if (task_list.size() != 0)
        {
            still=run_next(task_list);
        }
    }
}

int main(int argc, char *argv[])
{
try
    {    
    path starting_path=get_starting_path(argc,argv);
    Configuration a=Configuration(starting_path,path("/home/moky/.part_backup"));
    a.MakeBackup();
    //launching the thread that runs the tasks
    boost::thread sheduler( make_the_work, a.task_list );
    sheduler.join();
    }
catch (string err) { cout<<string("I got a bad news : ")<<err<<endl; }
}
