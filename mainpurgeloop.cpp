/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This is part of 'newbaka': you can redistribute it and/or modify
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
#include "mainbackuploop.h"
#include "mainpurgeloop.h"
#include "tasks.h"


MainPurgeLoop::MainPurgeLoop(){}

MainPurgeLoop::MainPurgeLoop(const path starting_path,const path backup_path,const path purge_path) : starting_path(starting_path),starting_backup_path(home_to_backup(starting_path)),backup_path(backup_path),home_path(getenv("HOME")),purge_removed_path(purge_path_to_purge_removed(purge_path)),purge_modified_path(purge_path_to_purge_modified(purge_path))
    {
        assert(  is_directory(starting_path) );
        assert(  is_directory(starting_backup_path) );
        assert(  is_directory(backup_path) );
        assert(  is_directory(purge_path) );

        create_tree(purge_modified_path);
        create_tree(purge_removed_path);

        assert( is_directory(purge_modified_path) );
        assert( is_directory(purge_removed_path) );
    }

void MainPurgeLoop::MakePurge()
{
    DealWithDirectory(starting_backup_path);
}

MainPurgeLoop::backup_to_home(const path backup_path) const
{
    string s_backup_path=backup_path.string();
    string s_purge_path=purge_path.string();
    string s_home=home_path.string();
    string s_return=backup_path.string();
    s_return.replace(0,s_purge_path.size(),s_home);
    return path(s_return);
}

MainPurgeLoop::DealWithDirectory(const path backup_path)
{
    assert(is_directory(backup_path));
    const path home_rep_path=backup_to_home(backup_path);
    cout<<"(purge) Le répertoire "<<backup_path<<"correspond à "<<home_rep_path;
    directory_iterator end_itr;
    for(  directory_iterator itr(backup_path); itr!=end_itr;++itr  )
    {
        path truc=backup_to_home(itr);
        cout<<"(purge) Le truc "<<backup_path<<"correspond à "<<home_rep_path;
    }a
}
