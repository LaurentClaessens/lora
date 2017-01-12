/*
Copyright 2015,2016 Laurent Claessens
contact : laurent@claessens-donadello.eu

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

#include "lora.h"
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
        }
    }
    cout<<"The work seems to be done. Leaving the 'task_runner' thread."<<endl;
}

void loops(Configuration* config_ptr)
{
    HashTable<string,MainLoop*> loops_table;
    loops_table["backup loop"]=new MainBackupLoop(config_ptr);
    loops_table["purge loop"]=new MainPurgeLoop(config_ptr);

    loops_table["backup loop"]->run();
    cout<<endl<<"Launching the purge process ..."<<endl;
    loops_table["purge loop"]->run();
    cout<<endl<<"all loops ended"<<endl;
}

int main(int argc, char* argv[])
{
    try
    {    
        Configuration* config_ptr=arguments_to_configuration(argc,argv);          // There is the file 'example.cfg' as example.

        //launching the thread that runs the tasks
        boost::thread task_runner( run_tasks, config_ptr );

        loops(config_ptr);
        
        task_runner.join();
        std::cout<<"task_runner terminated"<<std::endl;
    }
    catch (string err) { cout<<endl<<string("I got a bad news : ")<<err<<endl; }
    catch (std::length_error &err) 
    { 
        cerr<<"Caught : "<<err.what()<<endl;
        cerr<<"Type : "<<typeid(err).name()<<endl;
    }
    return 1;
}
