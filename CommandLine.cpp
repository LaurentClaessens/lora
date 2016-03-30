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

#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/algorithm/string/join.hpp>
#include "CommandLine.h"

using namespace std;


// COMMAND LINE

void CommandLine::setWorkingDirectory(path p) { working_directory=p; }
void CommandLine::setEnvironmentVariable(const string key,string value)
{
    environment_variables.setValue(key,value);
}

CommandLine::CommandLine():
    working_directory("."),
    in_terminal(false)
{};
CommandLine::CommandLine(string s):
    command(s),
    working_directory("."),
    environment_variables(),
    in_terminal(false)
{};

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
    cl.push_back( "cd \""+working_directory.string()+"\"  &&"  );
    cl.push_back( environment_variables.toString() );

    if (in_terminal) { cl.push_back(terminal_cl); }
    cl.push_back(command);
    return boost::algorithm::join(cl," ");
}

FILE* CommandLine::run()
{
    const string cl=full_command_line();
    FILE* in = popen(cl.c_str(), "r");
    if (!in)
    {
        throw std::string("Something got wrong launching the command "+cl);
    }
    return in;
}
   
string CommandLine::getOutput()
{
    char buff[512];
    string s="";
    FILE* in=run();
    while(fgets(buff, sizeof(buff), in)!=NULL)
    {
        s=s+buff;
    }
    pclose(in);
    return s;
}
   
// ENVIRONNEMENT VARIABLE

void CommandLine::EnvironmentVariables::setValue(const string key,string value)
{
    environment_variables.setValue(key,value);
}
string CommandLine::EnvironmentVariables::getValue(const string key) 
{
    return environment_variables[key];
}
string CommandLine::EnvironmentVariables::toString() const
{
    if (environment_variables.isEmpty()) { return ""; }

    vector<string> tmp;
    for (HashTable<string,string>::iterator itr=environment_variables.begin();itr!=environment_variables.end();itr++)
    {
        tmp.push_back(itr->key+"="+itr->value);
    }
    return boost::algorithm::join(tmp," ");
}

// TERMINAL LINE

TerminalLines::TerminalLines(const string _terminal,const string _in_terminal,const string _editor) :
    terminal(_terminal),
    in_terminal(_in_terminal),
    editor(_editor)
{}

string TerminalLines::getTerminal() const {return terminal;}
string TerminalLines::getInTerminal() const {return in_terminal;}
string TerminalLines::getEditor() const  {return editor;}



