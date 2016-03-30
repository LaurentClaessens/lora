/*
Copyright 2016 Laurent Claessens
contact : laurent@claessens-donadello.eu

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
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>     // starts_with
#include <QtGui>
#include "installation.h"

string DEFAULT_BOOST_THREAD_LIB = "/usr/lib/i386-linux-gnu/libboost_thread.so";
string DEFAULT_MOC_BIN="/usr/lib/i386-linux-gnu/qt4/bin/moc";

// ASK INFO


AskInfo::AskInfo(QString message)
{
    QVBoxLayout* layout=new QVBoxLayout();
    QLabel* text = new QLabel(message);
    edit = new QLineEdit();
    layout->addWidget(text);
    layout->addWidget(edit);
    setLayout(layout);
}

QString AskInfo::text() const { return edit->text(); }
void AskInfo::setText(QString message) { edit->setText(message); }

// DIRECTORY CHOOSER


void DirectoryChooser::choose_directory()
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

DirectoryChooser::DirectoryChooser(QString message) :
    QWidget()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* line_layout = new QHBoxLayout();

    edit = new QLineEdit();

    QPushButton* button = new QPushButton("Choose");

    connect(button,SIGNAL(clicked()),this,SLOT(choose_directory()));

    line_layout->addWidget(edit);
    line_layout->addWidget(button);

    if (message!="") { 
        QLabel* text = new QLabel(message);
        layout->addWidget(text); 
    }
    layout->addLayout(line_layout);
    setLayout(layout);
}

QString DirectoryChooser::text() { return edit->text();  }
void DirectoryChooser::setText(QString t) { return edit->setText(t);  }


// EXCLUDE CHOOSER


ExcludeChooser::ExcludeChooser(QString message) :
    QWidget(),
    text(message)
{
    choosers = vector<DirectoryChooser*>();
    setLayout(createLayout(text));
}

// the refresh process is complicated by the fact that we will
// delete all children, including the directory choosers. Thus we have to be able to
// recreate them.
void ExcludeChooser::refresh()
{
    // save the directories that are already sectioned
    QStringList dirs=getExcludedFiles();
    delete layout();
    qDeleteAll(this->children());

    // recreate the choosers
    choosers = vector<DirectoryChooser*>();
    for ( auto d:dirs  )
    {
        DirectoryChooser* ch = new DirectoryChooser("");
        ch->setText(d);
        choosers.push_back(ch);
    }
    setLayout( createLayout(text)  );
    update();
}

void ExcludeChooser::add_chooser()
{
    DirectoryChooser* chooser = new DirectoryChooser("");
    choosers.push_back(chooser);
    refresh();
}

QVBoxLayout* ExcludeChooser::createLayout(QString message) const
{
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* text = new QLabel(message);
    layout->addWidget(text);

    for (auto chooser:choosers) { layout->addWidget(chooser); }

    
    QToolButton* plus_button = new QToolButton();
    plus_button->setIcon(QIcon("plus.png"));
    connect(plus_button,SIGNAL(clicked()),this,SLOT(add_chooser()));
    
    layout->addWidget(plus_button);
    return layout;
}

QStringList ExcludeChooser::getExcludedFiles() const
{
    QStringList dirs;
    for ( auto c:choosers  )
    {
        dirs.push_back(c->text());
    }
    return dirs;
}

// BACKUP WIDGET

QString BackupWidget::getBackupDir() const { return backup_chooser->text(); }
QString BackupWidget::getPurgeDir() const { return purge_chooser->text(); }

QStringList BackupWidget::getExcludedFiles()  const
{ 
    return exclude_chooser->getExcludedFiles() ;
}

BackupWidget::BackupWidget() : 
    QWidget()
{
    QVBoxLayout* layout = new QVBoxLayout();

    backup_chooser = new DirectoryChooser( "Choose here the directory in which the backup has to be done. Ideally, it has to be an encrypted external disk. There must be as much free space as your HOME directory (minus the directories that you will not backup). ");
     purge_chooser = new DirectoryChooser("Choose here the purge directory. Ideally this is on an external encrypted disk, and should be next to the backup directory.");

    exclude_chooser= new ExcludeChooser("Choose here the directories to be excluded from backup.");
    layout->addWidget(backup_chooser);
    layout->addWidget(purge_chooser);
    layout->addWidget(exclude_chooser);
    setLayout(layout);
}

// TERMINAL WIDGET


TerminalWidget::TerminalWidget() :
    QWidget()
{
    QLabel* information = new QLabel("If you don't plan to use the git functionality, this section is facultative.");
    info_terminal = new AskInfo("Command line to launch a terminal (e.g. 'konsole', 'terminology', 'xterm')");
    info_in_terminal = new AskInfo("Command line to launch a program in a terminal (e.g. 'konsole -e', 'terminology -e', 'xterm -e')");
    info_editor = new AskInfo("My preferred editor (e.g. 'gedit', 'gvim', 'terminology -e vim', 'emacs')");

    QVBoxLayout* layout=new QVBoxLayout();
    layout->addWidget(information);
    layout->addStretch(); 
    layout->addWidget(info_terminal);
    layout->addWidget(info_in_terminal);
    layout->addWidget(info_editor);
    setLayout(layout);
}

QString TerminalWidget::getTerminal() const { return info_terminal->text(); }
QString TerminalWidget::getInTerminal() const { return info_in_terminal->text(); }
QString TerminalWidget::getEditor() const { return info_editor->text(); }

// COMPILAITION WIDGET


CompilationWidget::CompilationWidget():
    QWidget()
{
    QVBoxLayout* compilation_layout = new QVBoxLayout();

    QLabel* thread_text = new QLabel( "The makefile assumes presence of boost and in particular has the hard-coded path BOOST_THREAD_LIB  = /usr/lib/i386-linux-gnu/libboost_thread.so  (Ubutnu 15.10 Wily)  "  );
    thread_info = new AskInfo("path for libboost_thread.so");
    thread_info->setText(QString::fromStdString(DEFAULT_BOOST_THREAD_LIB));

    QLabel* moc_text = new QLabel( "The makefile assumes presence of the 'moc' program  (Ubutnu 15.10 Wily)  ");
    moc_info = new AskInfo("path for the moc binary");
    moc_info->setText(QString::fromStdString(DEFAULT_MOC_BIN));

    compilation_layout->addWidget(thread_text);
    compilation_layout->addWidget(thread_info);
    compilation_layout->addWidget(moc_text);
    compilation_layout->addWidget(moc_info);

    setLayout(compilation_layout);
}

QString CompilationWidget::getBOOST_THREAD_LIB() { return thread_info->text(); }
QString CompilationWidget::getMOC_BIN() { return moc_info->text(); }

// TAB WIDGET

TabWidget::TabWidget():
    QTabWidget()
{
    backup_tab=new BackupWidget();
    terminal_tab=new TerminalWidget();
    compilation_tab=new CompilationWidget();
    
    addTab(backup_tab,"About backup");
    addTab(terminal_tab,"Terminal preferences");
    addTab(compilation_tab,"Compilation preferences");
}

void TabWidget::write_backup_to_file() const
{
    std::ofstream output("lora.cfg");

    output<<"backup="<<backup_tab->getBackupDir().toUtf8().constData()<<std::endl;
    output<<"purge="<<backup_tab->getPurgeDir().toUtf8().constData()<<std::endl;
    output<<"starting="<<getenv("HOME")<<std::endl;

    //for (QString& s : backup_tab->getExcludedFiles()  )       //C++11
    QStringList qst = backup_tab->getExcludedFiles();
    if (qst.size()>0)
    {
        for (QStringList::iterator itr=qst.begin();itr!=qst.end();itr++  )      
        {
            QString s=*itr;
            output<<"exclude="<<s.toUtf8().constData()<<std::endl;
        }
    }
    output<<"terminal="<<terminal_tab->getTerminal().toStdString()<<std::endl;
    output<<"in_terminal="<<terminal_tab->getInTerminal().toStdString()<<std::endl;
    output<<"editor="<<terminal_tab->getEditor().toStdString()<<std::endl;
    output<<"log file="<<"lora.log"<<std::endl;
}

void TabWidget::write_compilation_to_file() const
{
    std::ifstream makefile("makefile");
    string output_filename="auto_makefile";
    std::ofstream tmp_file(output_filename.c_str(),std::ofstream::trunc);
    string line;

    while( getline(makefile,line)  )
    {
        tmp_file<<line<<std::endl; 
        if ( boost::algorithm::starts_with(line,"BOOST_THREAD_LIB  =")  ) 
        {
            tmp_file<<"BOOST_THREAD_LIB = "<<compilation_tab->getBOOST_THREAD_LIB().toStdString()<<std::endl;
        }
        if ( boost::algorithm::starts_with(line,"MOC_BIN =")  ) 
        {
            tmp_file<<"MOC_BIN = "<<compilation_tab->getMOC_BIN().toStdString()<<std::endl;
        }
    }
}

bool TabWidget::write_to_file()
{

    bool a1=true;
    bool a2=true;
    int ans =  QMessageBox::question(this,"Operation finished","This will write into 'lora.cfg'",QMessageBox::Save|QMessageBox::Cancel);
    if (ans==QMessageBox::Save) { write_backup_to_file() ; }
    if (ans==QMessageBox::Cancel) { a1=false;  }

    if (!(compilation_tab->getBOOST_THREAD_LIB().toStdString()==DEFAULT_BOOST_THREAD_LIB)  ||  (compilation_tab->getMOC_BIN().toStdString()==DEFAULT_MOC_BIN) )
    {
        int ans =  QMessageBox::question(this,"Operation finished","This will write into 'auto_makefile'",QMessageBox::Save|QMessageBox::Cancel);
        if (ans==QMessageBox::Save) { write_compilation_to_file() ; }
        if (ans==QMessageBox::Cancel) { a2=false;  }
    }
    return a1&&a2;
}

void TabWidget::do_finish()
{
    if (write_to_file()) { qApp->exit(); }
}

// MAIN WIDGET

MainWidget::MainWidget():
    QWidget()
{
    QVBoxLayout* main_vlayout = new QVBoxLayout();

    QHBoxLayout* finished_layout = new QHBoxLayout();
    QPushButton* finished_button = new QPushButton("Finished");
    finished_layout->addStretch();
    finished_layout->addWidget(finished_button);

    TabWidget* tab_widget = new TabWidget();

    main_vlayout->addWidget(tab_widget);
    main_vlayout->addLayout(finished_layout);

    connect(finished_button,SIGNAL(clicked()),tab_widget,SLOT(do_finish()));
    setLayout(main_vlayout);
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMainWindow* main_window = new QMainWindow();
    MainWidget* main_widget = new MainWidget();

    main_window->setCentralWidget(main_widget);

    main_window->show();
    return app.exec();
}
