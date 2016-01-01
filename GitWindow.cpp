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
#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <QtGui>
#include "GitWindow.h"

QString GitWindow::modified_text()
{
    vector<string> v_text;
    v_text.push_back("Modified files :");
    for (path f:repo.getModifiedFiles())
    {
        v_text.push_back("\t"+f.string());
    }
    string s_text=boost::algorithm::join(v_text,"\n");
    return QString::fromStdString( s_text  );
}

void GitWindow::addFormatButton(string format,QLayout* layout)
{
    FormatButton* button=new FormatButton(repo,format);
    layout->addWidget(button);
}

GitWindow::GitWindow(const GitRepository repo,const Configuration* conf,QWidget* parent):
    QDialog(parent),
    repo(repo),
    config_ptr(conf)
{ 
    QVBoxLayout* main_layout = new QVBoxLayout;
    QHBoxLayout* button_status_layout = new QHBoxLayout;
    QVBoxLayout* button_layout = new QVBoxLayout;
    QVBoxLayout* status_area_layout = new QVBoxLayout;
    QHBoxLayout* quick_commit_layout = new QHBoxLayout;

    QPushButton* git_diff_button=new QPushButton("See git diff");
    QPushButton* git_ignore_button=new QPushButton("Edit .gitignore");
    QPushButton* git_commit_button=new QPushButton("launch git commit -a");
    QPushButton* open_terminal_button=new QPushButton("Open a terminal here");

    addFormatButton("vim",button_layout);
    addFormatButton("latex",button_layout);
    addFormatButton("C++",button_layout);
    addFormatButton("python",button_layout);

    connect(git_commit_button,SIGNAL(clicked()),this,SLOT(launch_git_commit()));
    connect(open_terminal_button,SIGNAL(clicked()),this,SLOT(open_terminal()));
    connect(git_ignore_button,SIGNAL( clicked() ), this,SLOT( launch_edit_gitignore() ));

    QPushButton* quick_commit_button = new QPushButton("Commit me that");

    QLabel* modified_qlabel=new QLabel(modified_text());

    AddIgnoreLayout* add_ignore_layout = new AddIgnoreLayout(this);

    QPushButton* apply_add_ignore_button=new QPushButton("Apply these changes");
    connect(apply_add_ignore_button,SIGNAL(clicked()),this,SLOT(apply_add_ignore_changes()));

    status_area_layout->addWidget(modified_qlabel);
    status_area_layout->addLayout(add_ignore_layout);
    status_area_layout->addWidget(apply_add_ignore_button);

    button_status_layout->addLayout(button_layout);
    button_status_layout->addLayout(status_area_layout);
    main_layout->addLayout( button_status_layout  );
    main_layout->addLayout( quick_commit_layout  );

    button_layout->addWidget(git_diff_button);
    button_layout->addWidget(git_ignore_button);
    button_layout->addWidget(git_commit_button);
    button_layout->addWidget(open_terminal_button);
    quick_commit_layout->addWidget(quick_commit_button);
    
    quick_commit_button->setEnabled(false);

    connect(git_diff_button,SIGNAL( clicked() ), this,SLOT( launch_git_diff() ));

    setLayout(main_layout);
};

void GitWindow::apply_add_ignore_changes()
{
    for (auto itr=add_ignore_status.begin();itr!=add_ignore_status.end();itr++) 
    {
        path k=itr->key;
        UntrackedLine* line=itr->value;
        int v=line->getStatus();
        if (v==1) 
        {
            repo.git_add(itr->value->getActualPath());
            line->setEnabled(false);
        }
        if (v==2) 
        {
            repo.append_to_gitignore(itr->value->getActualPath());
            line->setEnabled(false);
        }
    }
}

AddIgnoreLayout::AddIgnoreLayout(GitWindow* gw):
    QVBoxLayout(),
    parent(gw)
{
    for (path f : parent->repo.getUntrackedFiles())
    {
        UntrackedLine* line=new UntrackedLine(f,parent);
        parent->add_ignore_status[f]=line;
        this->addLayout(line);
    }
}

UntrackedLine::UntrackedLine(path f, GitWindow* p):
    QHBoxLayout(),
    file(f),
    parent(p)
{
    printed_path=f.string();
    if (is_directory(parent->repo.getPath()/f))
    {
        std::cout<<"j'ajoute * à "<<f.string()<<std::endl;
        printed_path=printed_path+"*";
    }
    box_add=new QCheckBox("add");
    box_ignore=new QCheckBox("gitignore");
    box_noaction=new QCheckBox("no action");
    label_filename=new QLabel( QString::fromStdString(printed_path));
    box_noaction->setChecked(true);

    QButtonGroup* buttons = new QButtonGroup(this);
    buttons->addButton(box_add);
    buttons->addButton(box_ignore);
    buttons->addButton(box_noaction);
    buttons->setExclusive(true);

    this->addWidget(label_filename);
    this->addWidget(box_add);
    this->addWidget(box_ignore);
    this->addWidget(box_noaction);
}

void UntrackedLine::setEnabled(bool b)
{
    box_add->setEnabled(b);
    box_ignore->setEnabled(b);
    box_noaction->setEnabled(b);
}

int UntrackedLine::getStatus()
{
    if (box_add->isChecked()) { return 1 ;}
    if (box_ignore->isChecked()) { return 2 ;}
    if (box_noaction->isChecked()) { return 0 ;}
    throw std::string("One of these three boxes has to be checked");
}

string UntrackedLine::getActualPath() {return printed_path;}

void GitWindow::launch()
{
    this->show();
    qApp->exec();
}

void GitWindow::launch_git_diff() { repo.launchGitDiff(); }
void GitWindow::launch_git_commit() { repo.launchGitCommit(); }
void GitWindow::launch_edit_gitignore() { repo.editGitIgnore(); }
void GitWindow::open_terminal()
{
    CommandLine cl=CommandLine("");
    cl.setWorkingDirectory(repo.getPath());
    cl.setTerminal(config_ptr->getTerminal());
    cl.run();
}

// FORMAT BUTTON


FormatButton::FormatButton(GitRepository r,const string t) : 
    QPushButton(QString::fromStdString("add "+t+" to gitignore")),
    repo(r),
    format(t)
{
    connect(this,SIGNAL(clicked()),this,SLOT(add_to_gitignore()));
}

void FormatButton::add_to_gitignore() { repo.append_format_to_gitignore(format); }
