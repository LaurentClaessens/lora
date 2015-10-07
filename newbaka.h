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
//



// header technique from http://www.cplusplus.com/forum/articles/10627/
#ifndef __NEWBAKA_H_INCLUDED__
#define __NEWBAKA_H_INCLUDED__

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

void DealWithRepertory(path);
void DealWithFile(path);
bool create_tree(path);             // recursively creates the directory tree up to the given directory.
void my_copy_file(path ,path );     // copy 'from_path' to 'to_path' keeping the 'last_write_time' attribute.
void copy_tree(path,path);





#endif     // __NEWBAKA_H_INCLUDED__
