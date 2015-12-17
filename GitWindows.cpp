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

QHBoxLayout* GitWindows::untracked_line(path file)
{
    QCheckBox*  box_add = new QCheckBox("add");
    QCheckBox*  box_ignore = new QCheckBox("gitignore");
    QCheckBox*  box_noaction = new QCheckBox("no action");
    QLabel* filename = new QLabel( QString::fromStdString(file.string()));
    QHBoxLayout* line = new QHBoxLayout();

    box_noaction->setChecked(true);

    QButtonGroup* buttons = new QButtonGroup(line);
    buttons->addButton(box_add);
    buttons->addButton(box_ignore);
    buttons->addButton(box_noaction);
    buttons->setExclusive(true);

    line->addWidget(filename);
    line->addWidget(box_add);
    line->addWidget(box_ignore);
    line->addWidget(box_noaction);
    return line;
}

GitWindows::GitWindows(GitRepository repo,QWidget* parent):
    QDialog(parent),
    repo(repo)
{ 
    QVBoxLayout* main_layout = new QVBoxLayout;
    QHBoxLayout* button_status_layout = new QHBoxLayout;
    QVBoxLayout* button_layout = new QVBoxLayout;
    QVBoxLayout* status_area_layout = new QVBoxLayout;
    QVBoxLayout* modified_layout = new QVBoxLayout;
    QVBoxLayout* untracked_layout = new QVBoxLayout;
    QHBoxLayout* quick_commit_layout = new QHBoxLayout;

    QPushButton* git_diff_button=new QPushButton("git diff");
    QPushButton* git_ignore_button=new QPushButton("git ignore");
    QLabel* modified_qlabel=new QLabel(modified_text());


    for (path f : repo.getUntrackedFiles())
    {
        untracked_layout->addLayout(untracked_line(f));
    }

    QPushButton* quick_commit_button = new QPushButton("Ok! commit that.");

    modified_layout->addWidget(modified_qlabel);

    status_area_layout->addLayout(modified_layout);
    status_area_layout->addLayout(untracked_layout);

    button_status_layout->addLayout(button_layout);
    button_status_layout->addLayout(status_area_layout);
    main_layout->addLayout( button_status_layout  );
    main_layout->addLayout( quick_commit_layout  );

    button_layout->addWidget(git_diff_button);
    button_layout->addWidget(git_ignore_button);
    quick_commit_layout->addWidget(quick_commit_button);
    
    quick_commit_button->setEnabled(false);

    QObject::connect( git_diff_button,SIGNAL( clicked()  ), this,SLOT( launch_git_diff() )  );

    setLayout(main_layout);
};

void GitWindows::launch()
{
    this->show();
    qApp->exec();
}

void GitWindows::launch_git_diff() { repo.launchGitDiff(); }
