/*
Copyright 2015,2016 Laurent Claessens
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

#include <QtGui>
#include "lora.h"
#include "GitListWindow.h"
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

void run_tasks(Configuration* config_ptr)
{   
    TaskList* const tl_ptr=config_ptr->getTaskList();
    bool still=true;
    while (still)
    {
        if (tl_ptr->size() != 0)
        {
            still=run_next(*tl_ptr);
            config_ptr->processEvents();
        }
    }
    cout<<"The work seems to be done. Leaving the 'make_the_work' thread."<<endl;
}

void loops(Configuration* config_ptr)
{
    MainBackupLoop backup_loop=MainBackupLoop(config_ptr);
    MainPurgeLoop purge_loop=MainPurgeLoop(config_ptr);
    backup_loop.run();
    cout<<endl<<"Launching the purge process ..."<<endl;
    purge_loop.run();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    try
    {    
        Configuration* config_ptr=configuration_file_to_configuration(argc,argv);          // There is the file 'example.cfg' as example.

        GitListWindow* git_list_window=new GitListWindow(config_ptr);
        config_ptr->setGitListWindow(git_list_window);
        git_list_window->show();


        //launching the thread that runs the tasks
        boost::thread scheduler( run_tasks, config_ptr );

        loops(config_ptr);
        
        scheduler.join();
        git_list_window->join();
    }
    catch (string err) { cout<<endl<<string("I got a bad news : ")<<err<<endl; }
    catch (std::length_error &err) 
    { 
        cerr<<"Caught : "<<err.what()<<endl;
        cerr<<"Type : "<<typeid(err).name()<<endl;
    }
    return 1;
}


// segfault on what():  boost::filesystem::copy_file: Permesso negato:
// segfault when a non-existing directory is excluded.
