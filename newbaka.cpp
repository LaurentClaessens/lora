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
//
// Pour info : avant d'avoir séparé les déclarations et les implémentations, le fichier compilé fait : 336K newbaka   (commit df821ec8ed3ac1ed511e23ec24572b8fbef40ad5)
// Il en fait maintenant 252.


#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <boost/filesystem.hpp>             // the file filesystem.hpp is itself protected by #ifndef... #endif thus this is not included twice (by #include "newbaka.h")
#include <boost/thread.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "newbaka.h"
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

bool run_next(deque<GenericTask*> &task_list){
    bool ret;
    ret=task_list.front()->run();       // equivalent to   (*task_list.front()).run()
    task_list.pop_front();
    cout<<task_list.size()<<" tasks remaining"<<endl;
    return ret;
}

bool do_we_backup(path orig_path,path bak_path)
{
    assert(is_regular_file(orig_path));
    if (!is_regular_file(bak_path)){return true;}
    uintmax_t s_orig=file_size(orig_path);
    uintmax_t s_bak=file_size(bak_path);
    if (s_orig!=s_bak){return true;}
    time_t t_ori=last_write_time(orig_path);
    time_t t_bak=last_write_time(bak_path);
    if (t_ori>t_bak){return true;}
    if (t_ori<t_bak){throw string("The last_write_date of this file is f*cked up !"+orig_path.string()+" "+bak_path.string()+" ?");}
    return false;
}

Configuration read_configuration_file(const path cfg_path)
{
    assert(is_regular_file(cfg_path));
    ifstream cfg_file(cfg_path.c_str());
    string line;
    path bp,pp,sp;
    vector<string> parts;
    vector<path> exclude;
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
            else if (parts[0]=="exclude") {   exclude.push_back(one) ; }
            else { throw string("Unknown entry in the configuration file :"+parts[0]); }
            break;
        }
    }
    assert(is_directory(bp));
    assert(is_directory(pp));
    assert(is_directory(sp));

    Configuration config=Configuration(sp,bp,pp);
    config.add_exclude_path(exclude);

    return config;
}

Configuration::Configuration(){}

Configuration::Configuration(const path starting_path,const path backup_path,const path purge_rep_path) : starting_path(starting_path),backup_path(backup_path),home_path(getenv("HOME"))
    {
        assert(  is_directory(starting_path) );
        assert(  is_directory(backup_path) );
        assert(  is_directory(purge_rep_path) );

        boost::gregorian::date now=boost::gregorian::day_clock::local_day();
        const string sd=to_simple_string(now);
        this->purge_path = purge_rep_path/sd;
        create_tree(purge_path);
        assert( is_directory(purge_path) );
    }

path Configuration::home_to_backup(const path local_path) const
    {
        string spath=local_path.string();
        string shome=home_path.string();
        string sbackup=this->backup_path.string();
        spath.replace(0,shome.size(),sbackup);
        return path(spath);
    }
path Configuration::home_to_purge(const path local_path) const
    {
        string spath=local_path.string();
        string shome=home_path.string();
        string spurge=this->purge_path.string();
        spath.replace(0,shome.size(),spurge);
        return path(spath);
    }

void Configuration::add_exclude_path(const path ex)
{
    exclude_paths.push_back(ex);
}

void Configuration::add_exclude_path(vector<path> vp)
{
    for (  std::vector<path>::iterator it=vp.begin();it!=vp.end();++it  )
    {
        add_exclude_path(*it);
    }
}

void Configuration::DealWithFile(const path file_path) 
    {
        const path bak_path=this->home_to_backup(file_path);
        const path purge_path=this->home_to_purge(file_path);
        if (do_we_backup(file_path,bak_path))
        {
            assert( !boost::algorithm::starts_with(bak_path,this->starting_path ) );
            pathTriple triple;
            triple.orig=file_path;
            triple.bak=bak_path;
            triple.purge=purge_path;
            FileCopyTask*  ftask= new FileCopyTask(triple);
            task_list.push_back(ftask);
        }
    }

void Configuration::DealWithRepertory(const path rep_path) {
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
void Configuration::MakeBackup()
    { 
        const path sp=this->starting_path;
        create_tree(home_to_backup(sp));
        DealWithRepertory(sp); 
        FinalTask*  etask= new FinalTask();
        task_list.push_back(etask);
    }
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

void make_the_work(  deque<GenericTask*> &task_list)
{
    bool still=true;
    while (still)
    {
        if (task_list.size() != 0)
        {
            try{
             still=run_next(task_list);
               }
            catch (string err) { cout<<string("**** I got a bad news : ")<<err<<endl; }
        }
    }
}

int main(int argc, char *argv[])
{
try
    {    
    path starting_path=get_starting_path(argc,argv);
    Configuration a=read_configuration_file("newbaka.cfg");
    a.MakeBackup();
    //launching the thread that runs the tasks
    boost::thread sheduler( make_the_work, a.task_list );
    sheduler.join();
    }
catch (string err) { cout<<string("I got a bad news : ")<<err<<endl; }
}
