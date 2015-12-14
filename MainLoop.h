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

class MainLoop
{
    private:
        const path starting_path;
        const DirectoryConverter* const converter_ptr;
        TaskList* const converter_ptr;
    public :
        MainLoop(const path starting_path, const DirectoryConverter* const dc_ptr,TaskList* const tl_ptr);
        void loopOverDirectory(path sub_directory);
        virtual void DealWithDirectory(path)=0;
        virtual void DealWithFile(path)=0;
        virtual void run();
};

class MainBackupLoop: public MainLoop
{
    private:
        vector<path> excluded_paths;
    public:
        void add_exclude_path(const path);    
        void add_exclude_path(const vector<path>); 
        bool is_excluded(const path) const;       

        void DealWithDirectory(path);
        void DealWithFile(path);
        void run();
};

class MainPurgeLoop: public MainLoop
{
    public:
        void MainPurgeLoop();
        void DealWithDirectory(path);
        void DealWithFile(path);
};
