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

#include "tasks.h"
#include "MainLoop.h"
#include "Configuration.h"

using namespace boost::filesystem;
using namespace std;

bool run_next(TaskList &task_list)
{
    bool ret;
    ret=task_list.front()->run();
    delete task_list.front();
    task_list.pop_front();
    cout<<"["<<task_list.size()<<"]"<<endl;
    return ret;
}

void make_the_work(Configuration* config)
{   
    TaskList* const tl_ptr=config->getTaskList();
    bool still=true;
    while (still)
    {
        if (tl_ptr->size() != 0)
        {
            still=run_next(*tl_ptr);
        }
    }
    cout<<"The work seems to be done. Leaving the 'make_the_work' thread."<<endl;
}

int main(int argc, char *argv[])
{
    try
    {    
        path starting_backup_path=get_starting_backup_path(argc,argv);
        Configuration* config_ptr=read_configuration_file("backup.cfg",starting_backup_path);          // There is the file 'lora.cfg' as example.

        //launching the thread that runs the tasks
        boost::thread scheduler( make_the_work, config_ptr );

        MainBackupLoop backup_loop=MainBackupLoop(config_ptr);
        MainPurgeLoop purge_loop=MainPurgeLoop(config_ptr);

        backup_loop.run();
    
        cout<<endl<<"Launching the purge process ..."<<endl;
        purge_loop.run();

        cout<<"Let's wait the end of the tasks..."<<endl;
        scheduler.join();

    }
    catch (string err) { cout<<endl<<string("I got a bad news : ")<<err<<endl; }
    catch (std::length_error &err) 
    { 
        cerr<<"Caught : "<<err.what()<<endl;
        cerr<<"Type : "<<typeid(err).name()<<endl;
    }
}
