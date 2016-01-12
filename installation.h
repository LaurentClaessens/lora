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

CREDIT : the image "plus.png" is taken from Sage [sagemath.org]
            .Sage/src/doc/output/html/ca/intro/_static/plus.png

//*/


#include <string>
#include <vector>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTabWidget>

using std::string;
using std::vector;

// DirectoryChooser displays an explicative text and a directory chooser 
// (edit bar to write down and a button to choose).
// If the given text is "" no text is displayed.
class DirectoryChooser : public QWidget
{
    Q_OBJECT

    private slots :
        void choose_directory();
    private:
        QLineEdit* edit;
    public :
        DirectoryChooser(QString);
        QString text();
        void setText(QString);
};

class ExcludeChooser : public QWidget
{
    Q_OBJECT

    private slots:
        void add_chooser();
    private:
        vector<DirectoryChooser*> choosers;
        QVBoxLayout* createLayout(QString) const;
        QString text;
        void refresh();
    public:
        ExcludeChooser(QString);
        QStringList getExcludedFiles() const;
};

// The 'AskInfo' widget displays a text and ask for an answer.
class AskInfo : public QWidget
{
    Q_OBJECT 

    private :
        QLineEdit* edit;
    public:
        AskInfo(QString);   
        void setText(QString);
        QString text() const;
};

class BackupWidget : public QWidget
{

    Q_OBJECT

    private:
        string backup;
        QStringList exclude_list;
        DirectoryChooser* backup_chooser;
        DirectoryChooser* purge_chooser;
        ExcludeChooser* exclude_chooser;
    public:
        BackupWidget();
        QStringList getExcludedFiles() const;
        QString getBackupDir() const;
        QString getPurgeDir() const;
};

class TerminalWidget : public QWidget
{
    Q_OBJECT

    private:
        AskInfo* info_terminal;
        AskInfo* info_in_terminal;
        AskInfo* info_editor;
    public:
        TerminalWidget();
        QString getTerminal() const;
        QString getInTerminal() const;
        QString getEditor() const;
};

class CompilationWidget : public QWidget
{
    Q_OBJECT

    public:
        CompilationWidget();
};

class TabWidget : public QTabWidget
{
    Q_OBJECT

    private slots:
        void write_to_file() const;
    private:
        BackupWidget* backup_tab;
        TerminalWidget* terminal_tab;
        CompilationWidget* compilation_tab;
    public:
        TabWidget();
};

// The main widget contains the tabs and the "finished" button.
class MainWidget : public QWidget
{
    Q_OBJECT

    public:
        MainWidget();
};
