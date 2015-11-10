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

#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "lora.h"
#include "tasks.h"
#include "mainbackuploop.h"
#include "mainpurgeloop.h"

using namespace boost::filesystem;
using namespace std;

MainBackupLoop read_configuration_file(const path cfg_path,const path starting_path="")
{
    assert(is_regular_file(cfg_path));
    ifstream cfg_file(cfg_path.c_str());
    string line;
    path bp,pp,sp;
    const path home_path=path(getenv("HOME"));      // TODO : guess from the starting_path ??
    vector<string> parts;
    vector<path> exclude;
    vector<path> priority;
    while (  std::getline(cfg_file,line) )
    {
        split( parts,line,boost::is_any_of("=") );
        switch (parts.size())
        {
         case 0 : 
             throw string("Malformed configuration file : "+line); 
             break;
         case 1 :       // probably empty line
             break;
         case 2 :
            path one=path(parts[1]);
            if (parts[0]=="starting")  { sp=one; }
            else if (parts[0]=="backup") { bp=one; }
            else if (parts[0]=="purge") { pp=one; }
            else if (parts[0]=="exclude") 
            {   
                if (one.is_absolute()){ exclude.push_back(one) ;}
                else { exclude.push_back(  home_path/one  ) ; }
            }
            else if (parts[0]=="priority") 
            {   
                if (one.is_absolute()){ priority.push_back(one) ;}
                else { priority.push_back(  home_path/one  ) ; }
            }
            else { throw string("Unknown entry in the configuration file :"+parts[0]); }
            break;
        }
    }
    if (starting_path.string()!=""){sp=starting_path;}
    assert(is_directory(bp));
    assert(is_directory(pp));
    assert(is_directory(sp));

    cout<<"backup will be done in "<<bp<<endl;
    cout<<"purge will be done in  "<<pp<<endl;
    cout<<"starting directory is "<<sp<<endl;
    const DirectoryConverter* const converter_ptr=new DirectoryConverter(bp,pp);       //  the purge directories are created here.

    TaskList* tl_ptr=new TaskList();
    MainBackupLoop backup_loop=MainBackupLoop(sp,converter_ptr,tl_ptr);
    backup_loop.add_exclude_path(exclude);
    backup_loop.add_priority_path(priority);

    return backup_loop;
}

path get_starting_path(int argc, char *argv[])
{
    if (argc==1){return "";}
    const path starting_path=path(argv[1]);
    path full_path;
    if (starting_path.is_relative()) { full_path=absolute(starting_path); }
    else { full_path=starting_path; }
    full_path=canonical(full_path);
    cout<<"We are going to backup the repertory "<<full_path<<endl;
    return full_path;
}

bool run_next(TaskList &task_list)
{
    bool ret;
    ret=task_list.front()->run();
    delete task_list.front();
    task_list.pop_front();
    cout<<"["<<task_list.size()<<"]"<<endl;
    return ret;
}

void make_the_work(TaskList* tl_ptr)
{   
    bool still=true;
    while (still)
    {
        if (tl_ptr->size() != 0)
        {
            still=run_next(*tl_ptr);
        }
    }
    cout<<"The work seems to be done. Leaving the 'make_the_work' thread."<<endl;
    delete tl_ptr;
}

int main(int argc, char *argv[])
{
try
    {    
    path starting_path=get_starting_path(argc,argv);
    MainBackupLoop backup_loop=read_configuration_file("backup.cfg",starting_path);          // There is the file 'lora.cfg' as example.

    //launching the thread that runs the tasks
    boost::thread scheduler( make_the_work, backup_loop.get_task_list_ptr() );

    backup_loop.MakeBackup();
    
    cout<<endl<<"Launching the purge process ..."<<endl;
    MainPurgeLoop purge_loop=backup_loop.purge_loop();
    purge_loop.MakePurge();

    cout<<"Let's wait the end of the tasks..."<<endl;

    scheduler.join();

    }
catch (string err) { cout<<endl<<string("I got a bad news : ")<<err<<endl; }
catch (std::length_error &err) { 
    cerr<<"Caught : "<<err.what()<<endl;
    cerr<<"Type : "<<typeid(err).name()<<endl;
}
}
