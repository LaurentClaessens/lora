#include <iostream>
#include <string>
#include <QtGui>

#include "GitListWindow.h"

void GitListWindow::addGitButton(GitRepository repo)
{
    QString qs=QString::fromStdString(repo.getPathName());
    QPushButton* button=new QPushButton(qs);
    button->show();
    main_layout->addWidget(button);
    qApp->processEvents();
}

GitListWindow::GitListWindow():
    QMainWindow()
{
    main_layout=new QVBoxLayout;
    QPushButton* exit_button=new QPushButton("exit");
    setLayout(main_layout);
}
