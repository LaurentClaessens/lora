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
    QWidget* button_widget=new QWidget(this);
    QScrollArea* scroll_area=new QScrollArea;
    QPushButton* exit_button=new QPushButton("exit");

    main_layout->addWidget(exit_button);
    button_widget->setLayout(main_layout);

    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(button_widget);
    
    setCentralWidget(scroll_area);
}
