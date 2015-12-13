/*
Copyright 2015 Laurent Claessens
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

#include <QApplication>
#include <QPushButton>
#include "GitWindows.h"

GitWindows::GitWindows(GitRepository repo):repo(repo){};

 
void GitWindows::show()
{
    //QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ?");
    bouton.setText("Autre chose");
    bouton.show();

    QPushButton bouton2("Deuxième");
    bouton2.show();
}
