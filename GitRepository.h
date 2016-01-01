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

#ifndef   __GITREPOSITORY_H__
#define   __GITREPOSITORY_H__

#include "CommandLine.h"

using std::vector;
using std::string;

class GitRepository
{
    private :
        path repo_path;
        vector<string> v_commit_message() const; 
    public :
        GitRepository(path);

        path getPath() const;
        path getGitIgnoreFullPath() const;  // return the full path of .gitignore
        string getPathName() const;
        string getStatusMessage() const;
        bool isClean() const;   // if there is not subdir '.git', return "true"

        // The following functions are not marked as 'const' because
        // they change the git repository and then, indirectly, the logic of 'this'.
        // e.g. editing '.gitignore' can modify the return value of "isClean".
        void launchGitDiff(string terminal_launcher="konsole -e ");       // in a new terminal
        void launchGitCommit(string terminal_launcher="konsole -e ");    
        void append_to_gitignore(string);
        void append_format_to_gitignore(string);
        void editGitIgnore(string editor="konsole -e vim ");
        void git_add(string);
        void git_add(path);

        // the paths are relative to the repository path
        vector<path> getUntrackedFiles() const; 
        vector<path> getModifiedFiles() const;
};

#endif   //__GITREPOSITORY_H__
