/*
Copyright 2015 Laurent Claessens
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

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "DirectoryConverter.h"
#include "tasks.h"

using namespace boost::filesystem;
using namespace std;

path purge_path_to_purge_datetime(const path purge_path)
{
    time_t tt;
    time(&tt);
    struct tm* timeinfo=localtime(&tt);
    string s_year=std::to_string(timeinfo->tm_year+1900);

    string s_mon;
    switch (timeinfo->tm_mon)
    {
        case 0:
            s_mon="January";
            break;
        case 1:
            s_mon="February";
            break;
        case 2:
            s_mon="March";
            break;
        case 3:
            s_mon="April";
            break;
        case 4:
            s_mon="May";
            break;
        case 5:
            s_mon="June";
            break;
        case 6:
            s_mon="July";
            break;
        case 7:
            s_mon="August";
            break;
        case 8:
            s_mon="September";
            break;
        case 9:
            s_mon="October";
            break;
        case 10:
            s_mon="November";
            break;
        case 11:
            s_mon="December";
            break;
    }

    string s_mday=std::to_string(timeinfo->tm_mday);
    string s_hour=std::to_string(timeinfo->tm_hour);
    string s_min=std::to_string(timeinfo->tm_min);

    if (timeinfo->tm_min<10 ){ s_min="0"+s_min; }

    string s_date=s_year+"-"+s_mon+"-"+s_mday;
    string s_time=s_hour+"h"+s_min;

    return purge_path/s_date/s_time;
}   

DirectoryConverter::DirectoryConverter(const path bp,const path pp):
    home_path(getenv("HOME")),
    backup_path(bp),
    purge_path(pp),
    purge_datetime_path(purge_path_to_purge_datetime(pp)),
    purge_modified_path(purge_datetime_path/"modified"),
    purge_removed_path(purge_datetime_path/"removed")
{}

path DirectoryConverter::home_to_backup(const path local_path) const
{
    if(!boost::algorithm::starts_with(local_path,home_path) ) {throw string( "File '"+local_path.string()+"' does not belong to '"+home_path.string()+"'");}
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

    if (!boost::algorithm::starts_with(pathname,backup_path)) 
    {
        throw string( pathname.string()+" is not subdirectory of "+backup_path.string()  ); 
    }

    string s_backup_path=backup_path.string();
    string s_home=home_path.string();
    string s_return=pathname.string();

    s_return.replace(0,s_backup_path.size(),s_home);
    assert(  boost::algorithm::starts_with(s_return,s_home) );
    return path(s_return);
}
        
void DirectoryConverter::create_purge_directories() const
{
     Utilities().create_directory_tree(purge_modified_path);
     Utilities().create_directory_tree(purge_removed_path);
}

bool DirectoryConverter::verified_paths=false;

bool DirectoryConverter::are_all_paths_ok() const
{
    if (!verified_paths)
    {
        cout<<"Path existence verification"<<endl;
        if (!is_directory(home_path)){
            throw string( home_path.string()+" does not exist." ); 
            return false;}
        if (!is_directory(purge_path)){
            throw string( purge_path.string()+" does not exist." ); 
            return false;}
        if (!is_directory(backup_path)){
            throw string( backup_path.string()+" does not exist." ); 
            return false;}
        if (!is_directory(purge_datetime_path)){
            throw string( purge_datetime_path.string()+" does not exist." ); 
            return false;}
        if (!is_directory(purge_modified_path)){
            throw string( purge_modified_path.string()+" does not exist." ); 
            return false;}
        if (!is_directory(purge_removed_path)){
            throw string( purge_removed_path.string()+" does not exist." ); 
            return false;}
        verified_paths=true;
    }
    return true;
}
path DirectoryConverter::getBackupPath() const { return backup_path; }
path DirectoryConverter::getHomePath() const { return home_path; }
path DirectoryConverter::getPurgePath() const { return purge_path; }
