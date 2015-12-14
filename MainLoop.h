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

#ifndef __MAIN_LOOP_H__
#define __MAIN_LOOP_H__

#include <boost/filesystem.hpp>
#include "Configuration.h"

class MainLoop
{
    private:
        const path starting_path;
    protected:
        Configuration* const configuration;     // not const because one add tasks.
    public :
        MainLoop(Configuration*);
        void loopOverDirectory(path sub_directory);
        virtual void DealWithDirectory(path)=0;
        virtual void DealWithFile(path)=0;
        void create_purge_directories() const;
        virtual void run();
};

class MainBackupLoop: public MainLoop
{
    public:
        bool is_excluded(path);
        void DealWithDirectory(path);
        void DealWithFile(path);
        void run();
};

class MainPurgeLoop: public MainLoop
{
    public:
        MainPurgeLoop();
        void DealWithDirectory(path);
        void DealWithFile(path);
        void run();
};


#endif   //__MAIN_LOOP_H__
