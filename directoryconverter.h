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
// It creates the needed directories.
//
// An instance of this class is created buy the main backup loop and then given to the main purge loop.
class DirectoryConverter
{
    private :
        const path purge_path;
        const path backup_path;
        const path purge_modified_path;
        const path purge_removed_path;
        const path purge_datetime_path;
        const path home_path;
    public:
        DirectoryConverter();
        DirectoryConverter(const path,const path);       // backup path, purge_path
        path purge_to_purge_datetime(path);
        path purge_to_purge_modified(path);
        path purge_to_purge_removed(path);
        path local_to_backup(const path local_path) const;
        path local_to_modified_purge(const path local_path) const;            
}


#endif      //__DIRECTORY_CONVERTER_H__
