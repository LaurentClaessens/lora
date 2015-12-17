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
#include <QtGui>
#include "GitWindows.h"

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
    QTextEdit* status_area=new QTextEdit( QString( repo.getStatusMessage().c_str()  )  );
    QPushButton* quick_commit_button = new QPushButton("Ok! commit that.");

    button_status_layout->addLayout(button_layout);
    button_status_layout->addLayout(status_area_layout);
    main_layout->addLayout( button_status_layout  );
    main_layout->addLayout( quick_commit_layout  );

    button_layout->addWidget(git_diff_button);
    button_layout->addWidget(git_ignore_button);
    status_area_layout->addWidget(status_area);
    quick_commit_layout->addWidget(quick_commit_button);
    
    quick_commit_button->setEnabled(false);

    //GitDiffLauncher* git_diff_launcher= new GitDiffLauncher(repo);

    QObject::connect( git_diff_button,SIGNAL( clicked()  ), this,SLOT( launch_git_diff() )  );

    setLayout(main_layout);
};

void GitWindows::launch()
{
    this->show();
    qApp->exec();
}

void GitWindows::launch_git_diff() { repo.launchGitDiff(); }
