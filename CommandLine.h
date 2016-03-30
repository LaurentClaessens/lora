/*
Copyright 2015 Laurent Claessens
contact : laurent@claessens-donadello.eu

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
using std::string;

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
        FILE* run();        // don't forget to pclose the FILE* object.
        string getOutput();
};

// This class is basically a record of the command lines related to the terminal
// that are read from the configuration file.
// TODO : guess "in_terminal" and "editor" from terminal and a list of the more
//        classic terminals (terminolofy, konsole, xterm, ...)
//        Use the Debian's alternative system to get the editor. Or the git
//        preferences.
class TerminalLines
{
    private:
        const string terminal;
        const string in_terminal;
        const string editor;
    public:
        TerminalLines(const string,const string,const string);
        string getTerminal() const;
        string getInTerminal() const;
        string getEditor() const;
};

#endif // __COMMAND_LINE_H__
