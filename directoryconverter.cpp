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
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "directoryconverter.h"
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

path purge_path_to_purge_datetime(const path purge_path)
{
    assert(is_directory(purge_path));

    // std::to_string requires c++11. This is why we compile with g++ -std=c++11
    time_t tt;
    time(&tt);
    struct tm* timeinfo=localtime(&tt);
    string s_year=std::to_string(timeinfo->tm_year+1900);
    string s_mon=std::to_string(timeinfo->tm_mon+1);
    string s_mday=std::to_string(timeinfo->tm_mday+1);
    string s_hour=std::to_string(timeinfo->tm_hour);
    string s_min=std::to_string(timeinfo->tm_min);

    string s_date=s_year+"-"+s_mon+"-"+s_mday;
    string s_time=s_hour+"h"+s_min;

    return purge_path/s_date/s_time;
}   

DirectoryConverter::DirectoryConverter(const path home_path, const path backup_path, const path purge_path,const path purge_datetime_path,const path purge_modified_path,const path purge_removed_path ):
    home_path(home_path),
    backup_path(backup_path),
    purge_path(purge_path),
    purge_datetime_path(purge_datetime_path),
    purge_modified_path(purge_modified_path),
    purge_removed_path(purge_removed_path)
{
    create_purge_directories();
    assert(are_all_paths_ok());
}

path DirectoryConverter::home_to_backup(const path local_path) const
{
    assert(  boost::algorithm::starts_with(local_path,home_path) );
    const string s_home=home_path.string();
    const string s_backup=backup_path.string();
    string s_return=local_path.string();
    s_return.replace(0,s_home.size(),s_backup);
    assert(  boost::algorithm::starts_with(s_return,s_backup) );
    return path(s_return);
}

path DirectoryConverter::home_to_modified_purge(const path local_path) const
{
    assert(  boost::algorithm::starts_with(local_path,home_path) );
    const string s_home=home_path.string();
    const string s_purge=purge_modified_path.string();
    string s_return=local_path.string();
    s_return.replace(0,s_home.size(),s_purge);
    assert(  boost::algorithm::starts_with(s_return,s_purge) );
    return path(s_return);
}

path DirectoryConverter::backup_to_removed_purge(const path pathname) const
{
    assert(  boost::algorithm::starts_with(pathname,backup_path) );
    const string s_removed=purge_removed_path.string();
    const string s_backup=backup_path.string();
    string s_return=pathname.string();
    s_return.replace(0,s_backup.size(),s_removed);
    assert(  boost::algorithm::starts_with(s_return,s_removed) );
    return path(s_return);
}

path DirectoryConverter::backup_to_home(const path pathname) const
{
    string s_backup_path=pathname.string();
    string s_purge_path=purge_path.string();
    string s_home=home_path.string();
    string s_return=backup_path.string();
    s_return.replace(0,s_purge_path.size(),s_home);
    return path(s_return);
}
        
void DirectoryConverter::create_purge_directories() const
{
     create_tree(purge_modified_path);
     create_tree(purge_removed_path);
}

bool DirectoryConverter::are_all_paths_ok() const
{
    cout<<"Path existence verification"<<endl;
    if (!is_directory(home_path)){return false;}
    cout<<"home is ok"<<endl;
    if (!is_directory(purge_path)){return false;}
    cout<<"purge is ok"<<endl;
    if (!is_directory(backup_path)){return false;}
    cout<<"backup is ok"<<endl;
    if (!is_directory(purge_datetime_path)){return false;}
    cout<<"datetime is ok"<<endl;
    if (!is_directory(purge_modified_path)){return false;}
    cout<<"modified is ok"<<endl;
    if (!is_directory(purge_removed_path)){return false;}
    cout<<"removed is ok"<<endl;
    return true;
}

DirectoryConverter create_converter(const path bp,const path pp)
{

    assert(is_directory(bp));
    assert(is_directory(pp));

    path h_path=getenv("HOME");
    path p_datetime_path=purge_path_to_purge_datetime(pp);
    path p_modified_path=p_datetime_path/"modified";
    path p_removed_path=p_datetime_path/"removed";


    return DirectoryConverter( h_path,bp,pp,p_datetime_path,p_modified_path,p_removed_path );
}
