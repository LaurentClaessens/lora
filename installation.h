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

#include <string>
#include <vector>
#include <QLineEdit>
#include <QVBoxLayout>

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
        QVBoxLayout* createLayout(QString);
        QString text;
        void refresh();
    public:
        ExcludeChooser(QString);
};

class BackupWidget : public QWidget
{

    Q_OBJECT

    private:
        string backup;
        QStringList exclude_list;
    public:
        BackupWidget();

};

