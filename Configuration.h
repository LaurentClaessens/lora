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


// The aim of this class is to answer the questions which depend on the
// configuration (paths manipulation, excluded files, task_list, ...)
class Configuration
{
    friend make_the_work;
    private:
        path starting_path;
        const DirectoryConverter* converter_ptr;
        TaskList* const task_list_ptr;
        vector<path> excluded_paths;
    public:
        Configuration(path,DirectoryConverter*,TaskList*);
        void add_exclude_path(const path);    
        void add_exclude_path(const vector<path>); 

        void create_purge_directories();
        bool is_excluded(const path) const;       
}
