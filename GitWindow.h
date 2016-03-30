/*
Copyright 2015-2016 Laurent Claessens
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

#ifndef  __GIT_WINDOWS_H__
#define  __GIT_WINDOWS_H__

#include <QDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "GitRepository.h"
#include "HashTable.h"
#include "Configuration.h"


class UntrackedLine;
class StatusAreaLayout;

QString modified_text(GitRepository rep);


// The git repository 'repo' is not 'const' because the status of the repository
// is going to change (even if the logical object is probably const)

class GitWindow : public QDialog
{
    Q_OBJECT

    friend class AddIgnoreWidget;
    friend class UntrackedLine;

    private slots :
        void launch_git_diff();
        void launch_git_commit();
        void launch_edit_gitignore();
        void apply_add_ignore_changes();
        void open_terminal();
    private:
        GitRepository repo;
        const Configuration* config_ptr;
        HashTable<path,UntrackedLine*> add_ignore_status;

        QHBoxLayout* untracked_line(path file); 
        void addFormatButton(string,QLayout*);
        QVBoxLayout* createMainLayout();
    public:
        GitWindow(const GitRepository repo,const Configuration*,QWidget* parent=0);
        void updateMainLayout();
};

class AddIgnoreWidget : public QWidget
{
    Q_OBJECT

    private:
        QHBoxLayout* untracked_line(path);
        GitWindow* parent;
    public:
        AddIgnoreWidget(GitWindow*);
};

// 0 : no action. 1 : add, 2 : gitignore
class UntrackedLine : public QHBoxLayout
{
    Q_OBJECT

    private : 
        path file;
        string printed_path;   
        GitWindow* parent;
        QCheckBox*  box_add;
        QCheckBox*  box_ignore;
        QCheckBox*  box_noaction;
        QLabel* label_filename;
    public:
        UntrackedLine(path,GitWindow*);
        int getStatus();       // 0 : no action. 1 : add, 2 : gitignore

        // if 'file' is a directory, we want to work with "file/*"
        string getActualPath();
        void setEnabled(bool);
};

// A button that proposes to add files in .gitignore.
// The list of added files is based on the passed format (vim,latex,c++,etc.)
// See GitRepository::append_format_to_gitignore
class FormatButton : public QPushButton
{
    Q_OBJECT

    private slots:
        void add_to_gitignore();
    private :
        GitRepository repo;
        const string format;
        GitWindow* parent;
    public:
        FormatButton(GitRepository,string,GitWindow*);
};

// When the button of the quick line is clicked, QuickLine emits 
// the SIGNAL 'ckicked(QString)' with the text of the QLineEdit.
class QuickLine : public QWidget
{
    Q_OBJECT

    signals:
        void clicked(QString);
    private slots:
        void get_clicked();
    private:
        QLineEdit* edit;
        QPushButton* button;
    public:
        QuickLine(QString);
};

// Small bars that allows to
//  write a quick comment and launch 'git commit -a'
//  add files to the git repository
//  add lines to .gitignore
// Each of these action is presented by a QuickLine.
class QuickActions : public QWidget
{
    Q_OBJECT

    private:
        GitRepository repo;
        GitWindow* parent;
    private slots:
        void do_commit(QString);
        void do_add(QString);
        void do_ignore(QString);
    public:
        QuickActions(GitRepository,GitWindow*);
};

class StatusAreaLayout : public QVBoxLayout
{
    Q_OBJECT

    private :
        GitRepository repo;
        GitWindow* parent;

        QString modified_text();
    public:
        StatusAreaLayout(GitRepository,GitWindow*);
};


#endif   // __GIT_WINDOWS_H__
