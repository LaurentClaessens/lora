#include <iostream>
#include <string>
#include <QtGui>

#include<boost/thread.hpp>

#include "MainWindows.h"

// The Makefile is created by
// qmake -project
// qmake
// and that's it.

void MainWindows::add_button(std::string s)
{
    std::cout<<"Je passe par add_button "<<s<<std::endl;
    QPushButton* button= new QPushButton(  QString::fromStdString(s),this  );
    main_layout->addWidget( button );
    button->show();
    this->show();
}
 
MainWindows::MainWindows(QWidget* parent):
    QDialog(parent)
{
    add_button("un");
    setLayout(main_layout);
}
