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

// CONFIGURATION

void Configuration::create_purge_directories() const
{
    converter_ptr->create_purge_directories();
}

Configuration::Configuration(path sp,DirectoryConverte* dc,TaskList* tl):
    starting_path(sp),converter_ptr(dc),task_list_ptr(tl) {}

void MainBackupLoop::add_exclude_path(const path ex) 
{ 
    excluded_paths.push_back(ex); 
}

void Configuration::add_exclude_path(const std::vector<path> vp)
{
    for (auto p:vp) { add_exclude_path(p); }
}

bool Configuration::is_excluded(const path rep_path)

// CREATING FUNCTIONS (not to be confused with the constructor)

Configuration* read_configuration_file(const path cfg_path,const path starting_path="")
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

    const DirectoryConverter* const converter_ptr=new DirectoryConverter(bp,pp)
    TaskList* tl_ptr=new TaskList();
    
    Configuration* config_ptr = new Configuration( starting_path,converter_ptr,tl_ptr  );
    config->add_exclude_path(exclude);

    return config;
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
