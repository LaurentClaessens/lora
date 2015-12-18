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
#include <QCheckBox>
#include <QHBoxLayout>
#include "GitRepository.h"
#include "HashTable.h"

QString modified_text(GitRepository rep);

class GitWindows : public QDialog
{
    Q_OBJECT

    friend class FileQCheckBox;

    private slots :
        void launch_git_diff();
    private:
        // 0 : no action. 1 : add, 2 : gitignore
        HashTable<path,int> add_ignore_status;
        GitRepository repo;
        QHBoxLayout* untracked_line(path file); 
        QString modified_text();
    public:
        GitWindows(GitRepository repo,QWidget* parent=0);
        void launch();      // open a windows 
};

// FileQCheckBox is a QCheckBox that reminds the associated file and 
// its "signification".
// The FileQCheckedBox of "add" for file "foo.txt" has the charge of setting to 1 
// the "foo.txt" entry of the add_ignore_status table when it is checked.
// Constructor :
// text : the text printed in front of the CheckBox
// add_ignore_value : the value of the action associated with the box
// f : the file associated with the check box
// parent : the GitWindows in which the box in living (we need it to modify the add_ignore_status hash table).
class FileQCheckBox : public QCheckBox
{
    Q_OBJECT

    private:
        const path file;
        const int add_ignore_value;
        GitWindows* parent;
    private slots:
        void newCheckedValue(int);
    public:
        FileQCheckBox(const QString text,const int add_ignore_value,const path f,GitWindows* parent=0);
};


#endif   // __GIT_WINDOWS_H__
