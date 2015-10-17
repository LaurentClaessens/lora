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

#include <boost/filesystem.hpp>
#include <string>

using namespace boost::filesystem;
using namespace std;

path purge_path_to_purge_datetime(const path purge_path)
{
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

DirectoryConverter::DirectoryConverter(){};

DirectoryConverter::DirectoryConverter(const path backup_path,const path purge_path):backup_path(backup_path),purge_path(purge_path),purge_datetime_path(purge_path_to_purge_datetime(purge_path)),purge_modified_path(purge_datetime_path/"modified"),purge_removed_path(purge_datetime_path/"removed"),home_path(getenv("HOME"))     // purge_path  
{
    assert(is_directory(purge_path));
    assert(is_directory(home_path));

    create_tree(purge_modified_path);
    create_tree(purge_removed_path);

    assert(is_directory(purge_modified_path))
    assert(is_directory(purge_removed_path))
}

path DirectoryConverter::local_to_backup(const path local_path) const
{
    assert(  boost::starts_with(local_path,home_path) );
    const string shome=home_path.string();
    const string sbackup=backup_path.string();
    string s_return=local_path.string();
    s_return.replace(0,shome.size(),sbackup);
    assert(  boost::starts_with(s_return,backup_path) );
    return path(s_return);
}

path DirectoryConverter::local_to_modified_purge(const path local_path) const
{
    assert(  boost::starts_with(local_path,home_path) );
    const string shome=home_path.string();
    const string spurge=purge_modified_path.string();
    string s_return=local_path.string();
    s_return.replace(0,shome.size(),spurge);
    assert(  boost::starts_with(s_return,purge_modified_path) );
    return path(s_return);
}

path DirectoryConverter::backup_to_removed_purge(const path pathname) const
{
    assert(  boost::starts_with(pathname,backup_path) );
    const string s_removed=purge_removed_path.string();
    const string s_backup=backup_path.string();
    string s_return=pathname.string();
    s_return.replace(0,s_backup.size(),s_removed);
    assert(  boost::starts_with(s_return,purge_removed_path) );
    return path(s_return);
}
