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
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "GitRepository.h"

GitRepository::GitRepository(path p)
{
    // The whole point is that the user can enter "~/foo" and that
    // it seems that Boost is not able to convert ~ to the home dir name.
    string s_path = p.string();
    if (boost::algorithm::starts_with(s_path,"~"))
    {
        s_path.replace(0,1,getenv("HOME"));
    }
    repo_path=path(s_path);
}  

string GitRepository::getStatusMessage() const
{
    CommandLine cl=CommandLine("git status");
    cl.setEnvironmentVariable("LC_ALL","C");
    cl.setWorkingDirectory(getPath());
    return cl.getOutput();
}

path GitRepository::getPath() const {return repo_path;}

vector<string> GitRepository::v_commit_message() const
{
    string commit=getStatusMessage();
    vector<string> lines;
    boost::split(lines,commit,boost::is_any_of("\n"));
    return lines;
}

bool GitRepository::isClean()  const
{
    vector<string> lines=v_commit_message();
    string line;
    for (string& line:lines)
    {
        if (line=="nothing to commit, working directory clean") {return true;}
    }
    return false;
}

// this is a big bunch of sordid string manipulations.
// I don't even list the assumptions I made about the output of 'git status'
vector<path> GitRepository::getUntrackedFiles() const
{
    vector<string> lines=v_commit_message();
    vector<path> untracked_files;
    bool yet=false;
    string line;
    for (string& line:lines)
    {
        if (line=="Untracked files:") { yet=true;  }
        if (yet==true)
        {
            if ( boost::algorithm::starts_with(line,"\t")  ) 
            {
                string filename;
                vector<string> aux;
                boost::split(aux,line,boost::is_any_of("\t"));
                filename=aux[1];
                untracked_files.push_back(path(filename));
            }
        }
    }
    return untracked_files;
}

// I read somewhere that "If you really need to write crappy code, encapsulate it". 
// here is my modest contribution to that principle.
vector<path> GitRepository::getModifiedFiles() const
{
    vector<path> modified_files;
    vector<string> lines=v_commit_message();
    string prefix="\tmodified:   ";
    for (string& line:lines)
    {
        if (boost::algorithm::starts_with(line,prefix))
        {
            boost::algorithm::erase_all(line,prefix);
            modified_files.push_back(line);
        }
    }
    return modified_files;
}

void GitRepository::git_add(string s_file)
{
    CommandLine cl=CommandLine("git add \""+s_file+"\"");
    cl.setWorkingDirectory(getPath());
    cl.run();
}

void GitRepository::git_add(path file) { git_add(file.string()); }

path GitRepository::getGitIgnoreFullPath() const
{
    path full_filepath=getPath()/".gitignore";
    return full_filepath;
}

void GitRepository::append_to_gitignore(string s_file)
{
    std::ofstream filestream;
    path full_filepath=getGitIgnoreFullPath();
    filestream.open(full_filepath.string(),std::ios_base::app); 
    filestream<<"\n"<<s_file;
}

void GitRepository::append_to_gitignore(path file) { append_to_gitignore(file.string()); }

void GitRepository::launchGitDiff(string terminal_launcher)
{
    CommandLine cl=CommandLine("git diff");
    cl.setWorkingDirectory(getPath());
    cl.setTerminal(terminal_launcher);
    cl.run();
}

void GitRepository::editGitIgnore(string editor)
{
   CommandLine cl=CommandLine(editor+" .gitignore");
    cl.setWorkingDirectory(getPath());
    cl.run();
}
