/*
Copyright 2015-2016 Laurent Claessens
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

#include <string>
#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <QtGui>
#include "GitWindow.h"

using std::string;
void GitWindow::addFormatButton(string format,QLayout* layout)
{
    FormatButton* button=new FormatButton(repo,format,this);
    layout->addWidget(button);
}

QVBoxLayout* GitWindow::createMainLayout()
{
    add_ignore_status.reset();
    QVBoxLayout* main_layout = new QVBoxLayout();

    QLabel* title = new QLabel(QString::fromStdString( repo.getPathName() ));

    QHBoxLayout* button_status_layout = new QHBoxLayout;
    StatusAreaLayout* status_area_layout = new StatusAreaLayout(repo,this);

    QPushButton* git_diff_button=new QPushButton("See git diff");
    QPushButton* git_ignore_button=new QPushButton("Edit .gitignore");
    QPushButton* git_commit_button=new QPushButton("Launch git commit -a");
    QPushButton* open_terminal_button=new QPushButton("Open a terminal here");
    QPushButton* exit_button=new QPushButton("Exit");

    QFrame* format_frame = new QFrame();
    QVBoxLayout* format_layout = new QVBoxLayout();
    addFormatButton("vim",format_layout);
    addFormatButton("latex",format_layout);
    addFormatButton("C++",format_layout);
    addFormatButton("python",format_layout);
    format_frame->setLayout(format_layout);

    format_frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    format_frame->setLineWidth(2);

    QFrame* git_action_frame = new QFrame();
    QVBoxLayout* git_action_layout = new QVBoxLayout();
    git_action_layout->addWidget(git_diff_button);
    git_action_layout->addWidget(git_ignore_button);
    git_action_layout->addWidget(git_commit_button);
    git_action_frame->setLayout(git_action_layout);
    git_action_frame->setLayout(git_action_layout);

    git_action_frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    git_action_frame->setLineWidth(2);
   
    QVBoxLayout* buttons_layout = new QVBoxLayout();
    buttons_layout->addWidget(format_frame);
    buttons_layout->addWidget(git_action_frame);
    buttons_layout->addWidget(open_terminal_button);
    buttons_layout->addWidget(exit_button);

    connect(git_commit_button,SIGNAL(clicked()),this,SLOT(launch_git_commit()));
    connect(git_diff_button,SIGNAL( clicked() ), this,SLOT( launch_git_diff() ));
    connect(open_terminal_button,SIGNAL(clicked()),this,SLOT(open_terminal()));
    connect(exit_button,SIGNAL(clicked()),this,SLOT(close()));
    connect(git_ignore_button,SIGNAL( clicked() ), this,SLOT( launch_edit_gitignore() ));

    button_status_layout->addLayout(buttons_layout);
    button_status_layout->addLayout(status_area_layout);

    QuickActions* quick_actions = new QuickActions(repo,this);

    main_layout->addWidget(title);
    main_layout->addLayout( button_status_layout  );
    main_layout->addWidget( quick_actions  );

    return main_layout;
}

void GitWindow::updateMainLayout()
{
    auto* main_layout = createMainLayout();
    delete layout();
    qDeleteAll(this->children());
    setLayout(main_layout);
    update();
}

GitWindow::GitWindow(const GitRepository repo,const Configuration* conf,QWidget* parent):
    QDialog(parent),
    repo(repo),
    config_ptr(conf)
{ 
    auto* main_layout = createMainLayout();
    setLayout(main_layout);
};

void GitWindow::apply_add_ignore_changes()
{
    for (auto itr=add_ignore_status.begin();itr!=add_ignore_status.end();itr++) 
    {
        path k=itr->key;
        UntrackedLine* line=itr->value;
        int v=line->getStatus();
        if (v==1) 
        {
            repo.git_add(itr->value->getActualPath());
            line->setEnabled(false);
        }
        if (v==2) 
        {
            repo.append_to_gitignore(itr->value->getActualPath());
            line->setEnabled(false);
        }
    }
    updateMainLayout();
}

AddIgnoreWidget::AddIgnoreWidget(GitWindow* gw):
    QWidget(),
    parent(gw)
{
    QVBoxLayout*  layout = new QVBoxLayout();
    for (path f : parent->repo.getUntrackedFiles())
    {
        UntrackedLine* line=new UntrackedLine(f,parent);
        parent->add_ignore_status[f]=line;
        layout->addLayout(line);
    }
    setLayout(layout);
}

UntrackedLine::UntrackedLine(path f, GitWindow* p):
    QHBoxLayout(),
    file(f),
    parent(p)
{
    printed_path=f.string();
    if (is_directory(parent->repo.getPath()/f)) { printed_path=printed_path+"*"; }
    box_add=new QCheckBox("add");
    box_ignore=new QCheckBox("gitignore");
    box_noaction=new QCheckBox("no action");
    label_filename=new QLabel( QString::fromStdString(printed_path));
    box_noaction->setChecked(true);

    QButtonGroup* buttons = new QButtonGroup(this);
    buttons->addButton(box_add);
    buttons->addButton(box_ignore);
    buttons->addButton(box_noaction);
    buttons->setExclusive(true);

    addWidget(label_filename);
    addWidget(box_add);
    addWidget(box_ignore);
    addWidget(box_noaction);
}

void UntrackedLine::setEnabled(bool b)
{
    box_add->setEnabled(b);
    box_ignore->setEnabled(b);
    box_noaction->setEnabled(b);
}

int UntrackedLine::getStatus()
{
    if (box_add->isChecked()) { return 1 ;}
    if (box_ignore->isChecked()) { return 2 ;}
    if (box_noaction->isChecked()) { return 0 ;}
    throw string("One of these three boxes has to be checked");
}

string UntrackedLine::getActualPath() {return printed_path;}

void GitWindow::launch_git_diff() { repo.launchGitDiff(config_ptr->getInTerminal()); }
void GitWindow::launch_git_commit() { repo.launchGitCommit(config_ptr->getInTerminal()); }
void GitWindow::launch_edit_gitignore() { repo.editGitIgnore(config_ptr->getEditor()); }
void GitWindow::open_terminal()
{
    CommandLine cl=CommandLine("");
    cl.setWorkingDirectory(repo.getPath());
    cl.setTerminal(config_ptr->getTerminal());
    cl.run();
}


// FORMAT BUTTON


FormatButton::FormatButton(GitRepository r,const string t,GitWindow* gw) : 
    QPushButton(QString::fromStdString("add "+t+" to gitignore")),
    repo(r),
    format(t),
    parent(gw)
{
    connect(this,SIGNAL(clicked()),this,SLOT(add_to_gitignore()));
}

void FormatButton::add_to_gitignore() 
{ 
    repo.append_format_to_gitignore(format); 
    parent->updateMainLayout();
}


// QUICK LAYOUT

QuickLine::QuickLine(QString message):
    QWidget()
{
    QHBoxLayout* line = new QHBoxLayout();
    edit = new QLineEdit();
    button = new QPushButton(message);
    line->addWidget(edit);
    line->addWidget(button);
    connect( button,SIGNAL(clicked()),this,SLOT(get_clicked()) );
    setLayout(line);
}

void QuickLine::get_clicked() { emit clicked(edit->text()); }


QuickActions::QuickActions(GitRepository r,GitWindow* gw):
    QWidget(),
    repo(r),
    parent(gw)
{
    QVBoxLayout* quick_layout=new QVBoxLayout();

    QuickLine* track_line = new QuickLine("Track these files");
    QuickLine* ignore_line = new QuickLine("Ignore these files");
    QuickLine* commit_line = new QuickLine("Commit that");
    
    connect(track_line,SIGNAL(clicked(QString)),this,SLOT(do_add(QString)));
    connect(ignore_line,SIGNAL(clicked(QString)),this,SLOT(do_ignore(QString)));
    connect(commit_line,SIGNAL(clicked(QString)),this,SLOT(do_commit(QString)));

    quick_layout->addWidget(track_line);
    quick_layout->addWidget(ignore_line);
    quick_layout->addWidget(commit_line);

    setLayout(quick_layout);
}

void QuickActions::do_commit(QString message) 
{ 
    repo.commit(message.toStdString()); 
    parent->updateMainLayout();
}

void QuickActions::do_add(QString message) 
{ 
    repo.git_add(message.toStdString());
    parent->updateMainLayout();
}

void QuickActions::do_ignore(QString message) 
{ 
    repo.append_to_gitignore(message.toStdString());
    parent->updateMainLayout();
}

// STATUS AREA LAYOUT

QString StatusAreaLayout::modified_text()
{
    vector<string> v_text;
    auto modified_files=repo.getModifiedFiles();
    auto new_files=repo.getNewFiles();

    if (new_files.size()>0)
    {
        v_text.push_back("New files :");
        for (path f:repo.getNewFiles())
        {
            v_text.push_back("\t"+f.string());
        }
    }

    if( modified_files.size()>0 )
    {
        v_text.push_back("Modified files :");
        for (path f:modified_files)
        {
            v_text.push_back("\t"+f.string());
        }
    }

    string s_text=boost::algorithm::join(v_text,"\n");
    return QString::fromStdString( s_text  );
}


StatusAreaLayout::StatusAreaLayout(GitRepository r,GitWindow* p):
    QVBoxLayout(),
    repo(r),
    parent(p)
{
    QLabel* modified_qlabel=new QLabel(modified_text());
    AddIgnoreWidget* add_ignore_widget = new AddIgnoreWidget(parent);

    QScrollArea* add_ignore_scroll=new QScrollArea;

    QPushButton* apply_add_ignore_button=new QPushButton("Apply these changes");
    connect(apply_add_ignore_button,SIGNAL(clicked()),parent,SLOT(apply_add_ignore_changes()));

    addWidget(modified_qlabel);
    add_ignore_scroll->setWidget(add_ignore_widget);
    addWidget(add_ignore_scroll);

    addWidget(apply_add_ignore_button);
}
