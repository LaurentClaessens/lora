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

#ifndef __DIRECTORY_CONVERTER_H__
#define __DIRECTORY_CONVERTER_H__

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

// 'DirectoryConverter' serves to convert from one directory to the other between
// - purge                  (purge_path)
// - purge modified         (purge_modified_path)
// - purge removed          (purge_removed_path)
// - local file             
// - backup file            (backup_path)
//
// - starting_path              is the starting of a Main(Backup)(Purge)Loop.
//                              in one case, this is the purge path and in the other
//                              case, this is inside the home directory.
// - starting_backup_path       is the path of the subdirectory 
//                              of the home directory that has to be backuped.
// It creates the needed directories.


class DirectoryConverter
{
    private :                       // if one add some paths here, one has to update 'are_all_paths_ok' and maybe also 'create_purge_directories'.
        const path home_path;
        const path backup_path;
        const path purge_path;
        const path purge_datetime_path;
        const path purge_modified_path;
        const path purge_removed_path;
        static bool verified_paths;       // If one has already checked the paths.
    public:
        DirectoryConverter(const path backup_path, const path purge_path );
        DirectoryConverter() {};
        path getBackupPath() const;
        path getHomePath() const;
        path getPurgePath() const;
        void create_purge_directories() const;
        //path purge_to_purge_datetime(const path) const;
        //path purge_to_modified_purge(const path) const;
        //path purge_to_removed_purge(const path) const;
        path backup_to_removed_purge(const path) const;
        path backup_to_home(const path) const;
        path home_to_backup(const path) const;
        path home_to_modified_purge(const path) const;            
        bool are_all_paths_ok() const;                         // check if all the necessary directories exist.
};

DirectoryConverter create_converter(const path backup_path,const path purge_path);

#endif      //__DIRECTORY_CONVERTER_H__
