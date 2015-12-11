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


#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

#include <string>
#include <stdio.h>
#include <boost/filesystem.hpp>

#include "HashTable.h"

using namespace boost::filesystem;
using namespace std;

class CommandLine
{
    private:
        class EnvironmentVariables
        {
            private :
                HashTable<string,string>  environment_variables;
            public : 
                void setValue(const string,string);
                string getValue(const string);
                string toString() const;
        };

        std::string command;
        path working_directory=".";
        EnvironmentVariables environment_variables;
        bool in_terminal=false;
        string terminal_cl;  

        string full_command_line() const;
    public:
        void setWorkingDirectory(path);
        void setEnvironmentVariable(string,string);
        void setTerminal(string);       // the command will be launched in a new terminal. The given string is the command line that launch the terminal and expect for a command, e.g. "terminology -e" or "konsole -e".

        string toString() const;
        CommandLine();
        CommandLine(string);
        FILE* run();
        string getOutput();
        
};


#endif // __COMMAND_LINE_H__
