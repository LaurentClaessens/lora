#include <iostream>
#include <string>
#include <QtGui>

#include<boost/thread.hpp>

#include "MainWindows.h"

void MainWindows::addButton(std::string s)
{
    QPushButton* button= new QPushButton(  QString::fromStdString(s),this  );
    main_layout->addWidget( button );
    button->show();
    this->show();
}

MainWindows::MainWindows(QWidget* parent):
    QDialog(parent)
{
    addButton("un");
    setLayout(main_layout);
}
