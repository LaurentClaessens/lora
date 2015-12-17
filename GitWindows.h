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

#ifndef  __GIT_WINDOWS_H__
#define  __GIT_WINDOWS_H__

#include <QDialog>
#include <QHBoxLayout>
#include "GitRepository.h"

QString modified_text(GitRepository rep);

class GitWindows : public QDialog
{
    Q_OBJECT

    private slots :
        void launch_git_diff();
    private:
        GitRepository repo;
        QHBoxLayout* untracked_line(path file);
        QString modified_text();
    public:
        GitWindows(GitRepository repo,QWidget* parent=0);
        void launch();      // open a windows 
};

#endif   // __GIT_WINDOWS_H__
