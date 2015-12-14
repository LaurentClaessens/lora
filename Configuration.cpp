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
#include "Configuration.h"

// CONFIGURATION -- general

Configuration::Configuration(const path sp, const DirectoryConverter* const dc, TaskList* const  tl):
    starting_path(sp),converter_ptr(dc),task_list_ptr(tl) {}

path Configuration::getStartingPath() const { return starting_path; }

// CONFIGURATION -- directory converter

void Configuration::create_purge_directories() const { converter_ptr->create_purge_directories(); }
bool Configuration::are_all_paths_ok() const { return converter_ptr->are_all_paths_ok(); }
path Configuration::backup_to_removed_purge(const path rep) const { return converter_ptr->backup_to_removed_purge(rep); }
path Configuration::backup_to_home(const path rep) const { return converter_ptr->backup_to_home(rep); }
path Configuration::home_to_backup(const path rep) const {return converter_ptr->home_to_backup(rep); }
path Configuration::home_to_modified_purge(const path rep) const { return converter_ptr->home_to_modified_purge(rep); }


// CONFIGURATION -- excluding paths

void Configuration::add_exclude_path(const path ex) 
{ 
    excluded_paths.push_back(ex); 
}

void Configuration::add_exclude_path(const std::vector<path> vp)
{
    for (auto p:vp) { add_exclude_path(p); }
}

bool Configuration::is_excluded(const path rep_path) const
{
    for ( path p:excluded_paths  )
    {
        if (p==rep_path) {return true;}
    }
    return false;
} 

// CONFIGURATION -- task list

void Configuration::add_task(GenericTask* gt) { task_list_ptr->push_back(gt);  }

// CREATING FUNCTIONS (not to be confused with the constructor)

Configuration* read_configuration_file(const path cfg_path,const path starting_path="")
{
    assert(is_regular_file(cfg_path));
    std::ifstream cfg_file(cfg_path.c_str());
    std::string line;
    path bp,pp,sp;
    const path home_path=path(getenv("HOME"));      // TODO : guess from the starting_path ??
    std::vector<std::string> parts;
    std::vector<path> exclude;
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
            path one=path(parts[1]);
            if (parts[0]=="starting")  { sp=one; }
            else if (parts[0]=="backup") { bp=one; }
            else if (parts[0]=="purge") { pp=one; }
            else if (parts[0]=="exclude") 
            {   
                if (one.is_absolute()){ exclude.push_back(one) ;}
                else { exclude.push_back(  home_path/one  ) ; }
            }
            // Temporally we do not support priority.
            //else if (parts[0]=="priority") 
            //{   
             //   if (one.is_absolute()){ priority.push_back(one) ;}
             //   else { priority.push_back(  home_path/one  ) ; }
            //}
            else { throw std::string("Unknown entry in the configuration file :"+parts[0]); }
            break;
        }
    }
    if (starting_path.string()!=""){sp=starting_path;}
    assert(is_directory(bp));
    assert(is_directory(pp));
    assert(is_directory(sp));

    std::cout<<"backup will be done in "<<bp<<std::endl;
    std::cout<<"purge will be done in  "<<pp<<std::endl;
    std::cout<<"starting directory is "<<sp<<std::endl;

    const DirectoryConverter* const converter_ptr=new DirectoryConverter(bp,pp);
    TaskList* tl_ptr=new TaskList();
    
    Configuration* config_ptr = new Configuration( starting_path,converter_ptr,tl_ptr  );
    config_ptr->add_exclude_path(exclude);

    return config_ptr;
}

path get_starting_path(int argc, char *argv[])
{
    if (argc==1){return "";}
    const path starting_path=path(argv[1]);
    path full_path;
    if (starting_path.is_relative()) { full_path=absolute(starting_path); }
    else { full_path=starting_path; }
    full_path=canonical(full_path);
    std::cout<<"We are going to backup the repertory "<<full_path<<std::endl;
    return full_path;
}

// OTHER UTILITIES FUNCTIONS



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

