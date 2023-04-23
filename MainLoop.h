/*
Copyright 2015 Laurent Claessens
contact : laurent@claessens-donadello.eu

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
    protected:
        Configuration* const config_ptr;     // not const because one adds tasks.
    public :
        MainLoop(Configuration*);
        void loopOverDirectory(path sub_directory);
        virtual const path getStartingPath() const =0;
        virtual void DealWithDirectory(path)=0;
        virtual void DealWithFile(path)=0;
        virtual void run();
};

class MainBackupLoop: public MainLoop
{
    private:
        const path starting_path;
    public:
        MainBackupLoop(Configuration*);
        bool is_excluded(path);
        const path getStartingPath() const;
        void DealWithDirectory(path);
        void DealWithFile(path);
        void run();
};

class MainPurgeLoop: public MainLoop
{
    private:
        const path starting_path;
    public:
        MainPurgeLoop(Configuration*);
        const path getStartingPath() const;
        void DealWithDirectory(path);
        void DealWithFile(path);
        void run();
};

#endif   //__MAIN_LOOP_H__
