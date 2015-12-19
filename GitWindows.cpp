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
#include "GitWindows.h"

QString GitWindows::modified_text()
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

FileQCheckBox::FileQCheckBox(const QString text,const int ai_value,const path f,GitWindows* gw):
    QCheckBox(text),
    file(f),
    add_ignore_value(ai_value),
    parent(gw)
{ }

UntrackedLine::UntrackedLine(path f, GitWindows* p):
    QHBoxLayout(p),
    file(f),
    parent(p),
    box_add(FileQCheckBox("add",1,file,parent)),
    box_ignore(FileQCheckBox("gitignore",2,file,parent)),
    box_noaction(FileQCheckBox("no action",0,file,parent)),
    label_filename(QLabel( QString::fromStdString(file.string())))
{
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

    buttons.setEnable(false);
}

GitWindows::GitWindows(GitRepository repo,QWidget* parent):
    QDialog(parent),
    repo(repo)
{ 
    QVBoxLayout* main_layout = new QVBoxLayout;
    QHBoxLayout* button_status_layout = new QHBoxLayout;
    QVBoxLayout* button_layout = new QVBoxLayout;
    QVBoxLayout* status_area_layout = new QVBoxLayout;
    QHBoxLayout* quick_commit_layout = new QHBoxLayout;

    QPushButton* git_diff_button=new QPushButton("git diff");
    QPushButton* git_ignore_button=new QPushButton("git ignore");

    QPushButton* quick_commit_button = new QPushButton("Commit me that");

    QLabel* modified_qlabel=new QLabel(modified_text());
    AddIgnoreLayout* add_ignore_layout = new AddIgnoreLayout(this);

    QPushButton* apply_add_ignore_button=new QPushButton("ok for these changes");
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
    quick_commit_layout->addWidget(quick_commit_button);
    
    quick_commit_button->setEnabled(false);

    connect(git_diff_button,SIGNAL( clicked() ), this,SLOT( launch_git_diff() ));

    setLayout(main_layout);
};

void GitWindows::apply_add_ignore_changes()
{
    for (auto itr=add_ignore_status.begin();itr!=add_ignore_status.end();itr++) 
    {
        path k=itr->key;
        UntrackedLine* line=itr->value;
        int v=line->getStatus();
        if (v==1) 
        {
            repo.git_add(k);
            line->setEnable(false);
        }
        if (v==2) 
        {
            repo.append_to_gitignore(k);
            line->setEnable(false);
        }
    }
}

AddIgnoreLayout::AddIgnoreLayout(GitWindows* gw):
    QVBoxLayout(),
    parent(gw)
{
    for (path f : parent->repo.getUntrackedFiles())
    {
        line*=new UntrackedLine(f,parent);
        parent->add_ignore_status[file]=this;
        this->addLayout(line);
    }
}

UntrackedLine::setEnable(bool b)
{
    box_add.setEnable(b);
    box_ignore.setEnable(b):
    box_noaction.setEnable(b);
}

UntrackedLine::getStatus()
{
    if (box_add->isChecked()) { return 1 ;}
    if (box_ignore->isChecked()) { return 2 ;}
    if (box_noaction->isChecked()) { return 0 ;}
    throw std::string("One of these three boxes has to be checked");
}

void GitWindows::launch()
{
    this->show();
    qApp->exec();
}

void GitWindows::launch_git_diff() { repo.launchGitDiff(); }
