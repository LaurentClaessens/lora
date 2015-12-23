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

#include <QtGui>
#include "lora.h"
#include "MainWindows.h"

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
 
void start_main_windows(MainWindows* mw)  {mw->exec();}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    try
    {    

        MainWindows* main_windows=new MainWindows();
        boost::thread main_windows_process( start_main_windows, main_windows );

        path starting_backup_path=get_starting_backup_path(argc,argv);
        Configuration* config_ptr=configuration_file_to_configuration("lora.cfg",starting_backup_path);          // There is the file 'example.cfg' as example.

        config_ptr->setMainWindows(main_windows);
        //launching the thread that runs the tasks
        boost::thread scheduler( run_tasks, config_ptr );

        loops(config_ptr);
        
        scheduler.join();
    }
    catch (string err) { cout<<endl<<string("I got a bad news : ")<<err<<endl; }
    catch (std::length_error &err) 
    { 
        cerr<<"Caught : "<<err.what()<<endl;
        cerr<<"Type : "<<typeid(err).name()<<endl;
    }
    return 1;
}
