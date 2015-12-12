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

class GitRepository
{
    private :
        path repo_path;
        vector<string> v_commit_message();
    public :
        GitRepository(path);

        path  getPath();
        string getPathName();
        string getStatusMessage();
        bool isClean();
        void launchGitDiff();       // in a new terminal

        // in the paths are relative to the repository path
        vector<path> getUntrackedFiles(); 
        vector<path> getModifiedFiles();
};

#endif   //__GITREPOSITORY_H__
