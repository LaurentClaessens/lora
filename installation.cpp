/*
Copyright 2016 Laurent Claessens
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

#include <iostream>
#include <QtGui>
#include "installation.h"

void DirectoryChoosingLine::choose_directory()
{
    QFileDialog* fdialog = new QFileDialog(this);
    fdialog->setFileMode(QFileDialog::Directory);
    fdialog->setOption(QFileDialog::ShowDirsOnly,true);
    QStringList fileNames;
    if (fdialog->exec())
    {
        fileNames = fdialog->selectedFiles();
    }
    edit->setText(fileNames.at(0));
}

DirectoryChoosingLine::DirectoryChoosingLine() :
    QWidget()
{
    QHBoxLayout* layout = new QHBoxLayout();

    edit = new QLineEdit();

    QPushButton* button = new QPushButton("Choose");

    connect(button,SIGNAL(clicked()),this,SLOT(choose_directory()));

    layout->addWidget(edit);
    layout->addWidget(button);
    setLayout(layout);
}

QString DirectoryChoosingLine::text() { return edit->text();  }

BackupWidget::BackupWidget() : 
    QWidget()
{
    QVBoxLayout* layout = new QVBoxLayout();
    
    QLabel* text = new QLabel( "This is the explanation text"  );
    DirectoryChoosingLine* backup_line = new DirectoryChoosingLine();
    layout->addWidget(text);   
    layout->addWidget(backup_line);

    setLayout(layout);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow* main_window = new QMainWindow();

    QTabWidget* tab_widget=new QTabWidget();
    BackupWidget* backup_tab=new BackupWidget();
    QLabel* git_tab=new QLabel("deux");
    QLabel* terminal_tab=new QLabel("deux");
    QLabel* compilation_tab=new QLabel("deux");
    tab_widget->addTab(backup_tab,"About backup");
    tab_widget->addTab(git_tab,"About git");
    tab_widget->addTab(terminal_tab,"Terminal preferences");
    tab_widget->addTab(compilation_tab,"Compilation preferences");

    main_window->setCentralWidget(tab_widget);

    main_window->show();
    return app.exec();
}
