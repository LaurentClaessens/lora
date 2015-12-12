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

#include <string>
#include <boost/algorithm/string.hpp>
#include "GitRepository.h"

GitRepository::GitRepository(path p): repo_path(p) {}  

string GitRepository::getStatusMessage()
{
    CommandLine cl=CommandLine("git status");
    cl.setEnvironmentVariable("LC_ALL","C");
    cl.setWorkingDirectory(getPath());
    return cl.getOutput();
}

path GitRepository::getPath() {return repo_path;}
bool GitRepository::isClean() 
{
    string commit=getStatusMessage();
    string line;
    vector<string> lines;
    boost::split(lines,commit,boost::is_any_of("\n"));
    for ( vector<string>::iterator itr=lines.begin();itr!=lines.end();itr++ )
    {
        line=*itr;
        if (line=="nothing to commit, working directory clean") {return true;}
    }
    return false;
}
