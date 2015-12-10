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

#include <iostream>
#include <string>
#include <boost/algorithm/string/join.hpp>
#include "CommandLine.h"

using namespace std;


void CommandLine::setWorkingDirectory(path p) { working_directory=p; }
void CommandLine::setEnvironmentVariable(const string key,string value)
{
    environment_variables.setValue(key,value);
}

CommandLine::CommandLine(){};
CommandLine::CommandLine(string s):command(s) {};

string CommandLine::toString() const
{
    return full_command_line();
}

void CommandLine::setTerminal(string t) { 
    in_terminal=true;
    terminal_cl=t ;
};

string CommandLine::full_command_line() const
{
    vector<string> cl;
    cl.push_back( "cd "+working_directory.string()+"&&"  );
    cl.push_back( environment_variables.toString() );

    if (in_terminal) { cl.push_back(terminal_cl); }
    cl.push_back(command);
    return boost::algorithm::join(cl," ");
}

void CommandLine::EnvironmentVariables::setValue(const string key,string value)
{
    environment_variables[key]=value;
}
string CommandLine::EnvironmentVariables::getValue(const string key) 
{
    return environment_variables[key];
}
string CommandLine::EnvironmentVariables::toString() const
{
    vector<string> tmp;
    for (auto itr=environment_variables.begin();itr!=environment_variables.end();itr++)
    {
        tmp.push_back( itr->first+"="+itr->second);
    }
    return boost::algorithm::join(tmp," ");
}
