/*
Copyright 2016 Laurent Claessens
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


#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

using boost::filesystem::path;

// This class is clearly intended to be virtual in a next future. This should allow 
// other logging ways than writing in a file.

class Logging
{
    private:
        std::ofstream log_stream;        // closed when destroyed, then the
                                    // default destructor is sufficient.
    public:
        void setFile(path);
        void write(std::string);
};

#endif     //__LOGGING_H__

