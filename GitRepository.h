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

#ifndef   __GITREPOSITORY_H__
#define   __GITREPOSITORY_H__

#include "CommandLine.h"

using std::vector;
using std::string;

// About GitRepository::isClean().
// if there is not subdir '.git', return "true"
// If all modified files begin with "../", return true. Here is why :
//    Let ~/.kde be a gitted directory : ~/.kde/.git exists and IS a git repository.
//    Then the directory 
//       ~/.kde/share/apps/konqueror/view_properties/.kde/.git
 //   may exist. But this one IS NOT a git repository.
 //   When the backup loop arrives at that directory, it tests "git status" and
 //   gets all the ../../../../<any file modified in ~/.kde>.
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
        bool isClean() const;   

        // The following functions are not marked as 'const' because
        // they change the git repository and then, indirectly, the logic of 'this'.
        // e.g. editing '.gitignore' can modify the return value of "isClean".
        void launchGitDiff(string terminal_launcher="konsole -e ");       // in a new terminal
        void append_to_gitignore(string);
        void append_format_to_gitignore(string);
        void editGitIgnore(string editor="konsole -e vim ");
        void git_add(string);
        void git_add(path);
        
        //launchGitCommit launches 'git commit -a' in a terminal
        //commit simply performs 'git commit -a' with the given message
        void launchGitCommit(string terminal_launcher="konsole -e ");    
        void commit(string message);

        // the paths are relative to the repository path
        vector<path> getUntrackedFiles() const; 
        vector<path> getModifiedFiles() const;
        vector<path> getNewFiles() const;
};

#endif   //__GITREPOSITORY_H__
