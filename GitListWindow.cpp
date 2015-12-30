#include <iostream>
#include <string>
#include <QtGui>

#include "GitListWindow.h"
#include "GitWindow.h"


// GIT BUTTON

GitButton::GitButton(GitRepository r) : 
    QPushButton(QString::fromStdString(r.getPathName())), 
    repo(r) 
    {}

void GitButton::launchGitWindow()
{
    std::cout<<"Clicked for git window in "<<repo.getPathName()<<std::endl;
    GitWindow* git_window = new GitWindow(repo,this);
    git_window->show();
    git_window->activateWindow();
    qApp->processEvents();
}

// GIT LIST WINDOW

void GitListWindow::addGitButton(GitRepository repo)
{
    GitButton* button=new GitButton(repo);

    connect(button,SIGNAL(clicked()),button,SLOT(launchGitWindow()));
    button->show();
    main_layout->addWidget(button);
    qApp->processEvents();
}

GitListWindow::GitListWindow():
    QMainWindow()
{
    main_layout=new QVBoxLayout;
    QWidget* button_widget=new QWidget(this);
    QScrollArea* scroll_area=new QScrollArea;
    QPushButton* exit_button=new QPushButton("exit");

    connect( exit_button,SIGNAL(clicked()),this,SLOT(close())  );

    main_layout->addWidget(exit_button);
    button_widget->setLayout(main_layout);

    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(button_widget);
    
    setCentralWidget(scroll_area);
}
