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

#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "GitListWindow.h"
#include "Configuration.h"

using std::string;

// CONFIGURATION -- general

Configuration::Configuration(const path sp, const DirectoryConverter* const dc, TaskList* const  tl, const TerminalLines* const term_lines_ptr):
    starting_backup_path(sp),
    converter_ptr(dc),
    task_list_ptr(tl),
    terminal_lines_ptr(term_lines_ptr)
{}

path Configuration::getStartingBackupPath() const { return starting_backup_path; }

// CONFIGURATION -- directory converter

void Configuration::create_purge_directories() const { converter_ptr->create_purge_directories(); }
bool Configuration::are_all_paths_ok() const { return converter_ptr->are_all_paths_ok(); }
path Configuration::backup_to_removed_purge(const path rep) const { return converter_ptr->backup_to_removed_purge(rep); }

path Configuration::backup_to_home(const path rep) const
{
    return converter_ptr->backup_to_home(rep); 
}
path Configuration::home_to_backup(const path rep) const {return converter_ptr->home_to_backup(rep); }
path Configuration::home_to_modified_purge(const path rep) const { return converter_ptr->home_to_modified_purge(rep); }

path Configuration::getHomePath() const { return converter_ptr->getHomePath(); }
path Configuration::getPurgePath() const { return converter_ptr->getPurgePath(); }
path Configuration::getBackupPath() const { return converter_ptr->getBackupPath(); }

// CONFIGURATION -- excluding paths

void Configuration::add_exclude_path(const path ex) 
{ 
    path can_ex=canonical(ex);
    excluded_paths.push_back(can_ex); 
}

void Configuration::add_exclude_path(const std::vector<path> vp)
{
    for (auto p:vp) { add_exclude_path(p); }
}

bool Configuration::is_excluded(const path rep_path) const
{
    if (!exists(rep_path)) {return true;}
    for ( path p:excluded_paths )
    {
        if (p==canonical(rep_path)) {return true;}
    }
    return false;
} 

// CONFIGURATION -- task list

void Configuration::add_task(GenericTask* gt) { task_list_ptr->push_back(gt);  }
TaskList* Configuration::getTaskList() const {return task_list_ptr;}

// CREATING FUNCTIONS (not to be confused with the constructor)

HashTable<std::string,std::vector<std::string>> read_configuration_file(const path cfg_path)
{
    assert(is_regular_file(cfg_path));
    auto hash_table=HashTable<std::string,std::vector<std::string>>();
    std::ifstream cfg_file(cfg_path.c_str());
    std::string line;
    std::vector<std::string> parts;
    while (  std::getline(cfg_file,line) )
    {
        split( parts,line,boost::is_any_of("=") );
        switch (parts.size())
        {
         case 0 : 
             throw std::string("Malformed configuration file : "+line); 
             break;
         case 1 :       // probably empty line
             break;
         case 2 :
             std::string key=parts[0];
             std::string value=parts[1];
            if (!hash_table.isKey(key))
            {
                hash_table[key]=std::vector<std::string>();
            }
            hash_table[key].push_back(value);
            break;
        }
    }
    return hash_table;
}

std::string read_configuration_file(const path cfg_path,const std::string searched_property)
{
    auto hash_table=read_configuration_file(cfg_path);

    for (auto itr=hash_table.begin();itr!=hash_table.end();itr++)
    {
        if (itr->key==searched_property)
        {
            std::vector<std::string> v=itr->value;
            if (!v.empty()) { return v.back(); }
        }
    }
    throw std::string("Property not found in the configuration file : "+searched_property);
}

HashTable<string,string> parse_arguments(int argc,char* argv[])
{
    HashTable<string,string>hash=HashTable<string,string>();
    hash["--configuration"]="lora.cfg";
    hash["--starting"]="~";
    for (int i=0;i<argc;i++)
    {
        std::vector<string> parts;
        string arg=argv[i];
        split( parts,arg,boost::is_any_of("=") );
        hash[parts[0]]=parts[1];
    }
    return hash;
}

path get_starting_backup_path(string s_path)
{
    const path starting_path=path(s_path);
    path full_path;
    if (starting_path.is_relative()) { full_path=absolute(starting_path); }
    else { full_path=starting_path; }
    full_path=canonical(full_path);
    std::cout<<"We are going to backup the repertory "<<full_path<<std::endl;
    return full_path;
}

Configuration* configuration_file_to_configuration(int argc, char* argv[],bool verbose)
{

    HashTable<string,string> hash_args=parse_arguments(argc,argv);

    path cfg_path=hash_args["--configuration"];
    path starting_backup_path=get_starting_backup_path(hash_args["--starting"]);

    assert(is_regular_file(cfg_path));
    auto hash_table=read_configuration_file(cfg_path);

    path bp,pp,sp;
    const path home_path=path(getenv("HOME"));
    std::vector<path> exclude;

    bp=hash_table["backup"][0];
    pp=hash_table["purge"][0];
    sp=hash_table["starting"][0];

    for (std::string s_path:hash_table["exclude"])
    {
        path aux=path(s_path);
        if (aux.is_absolute()){ exclude.push_back(aux) ;}
        else { exclude.push_back(  home_path/aux ) ; }
    }
    if (verbose)
    {
        std::cout<<"backup will be done in "<<bp<<std::endl;
        std::cout<<"purge will be done in  "<<pp<<std::endl;
        std::cout<<"starting directory is "<<sp<<std::endl;
    }

    if (starting_backup_path.string()!=""){sp=starting_backup_path;}

    string terminal=hash_table["terminal"][0];
    string in_terminal=hash_table["in_terminal"][0];
    string editor=hash_table["editor"][0];

    const DirectoryConverter* const converter_ptr=new DirectoryConverter(bp,pp);
    TaskList* tl_ptr=new TaskList();
    const TerminalLines* const terminal_lines_ptr=new TerminalLines(terminal,in_terminal,editor);
    
    Configuration* config_ptr = new Configuration( sp,converter_ptr,tl_ptr,terminal_lines_ptr  );
    config_ptr->add_exclude_path(exclude);

    return config_ptr;
}

// GIT LIST WINDOWS

void Configuration::setGitListWindow(GitListWindow* gw)
{
    git_list_window=gw;
}

void Configuration::addGitButton(GitRepository repo)
{
    git_list_window->addGitButton(repo);
}

// TERMINAL LINES

const string Configuration::getTerminal() const {return terminal_lines_ptr->getTerminal();}
const string Configuration::getInTerminal() const {return terminal_lines_ptr->getInTerminal();}
const string Configuration::getEditor() const {return terminal_lines_ptr->getEditor();}

// OTHER UTILITIES FUNCTIONS

bool Configuration::do_we_backup(const path orig_path,const path bak_path) const
{
    assert(is_regular_file(orig_path));
    if (!is_regular_file(bak_path)){return true;}
    uintmax_t s_orig=file_size(orig_path);
    uintmax_t s_bak=file_size(bak_path);
    if (s_orig!=s_bak){return true;}
    time_t t_ori=last_write_time(orig_path);
    time_t t_bak=last_write_time(bak_path);
    if (t_ori>t_bak){return true;}

    if (t_ori<t_bak){throw std::string("The last_write_date of this file is f*cked up !"+orig_path.string()+" "+bak_path.string()+" ?");}

    // The following piece is in order to correct an error in the time managing
    // of my previous backup program.
    /*
    if (t_ori<t_bak){
        std::cout<<"Updating the date of "<<bak_path<<std::endl;
        last_write_time( bak_path,t_ori );
    }
    //*/

    return false;
}

