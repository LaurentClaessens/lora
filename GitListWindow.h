#ifndef __GIT_LIST_WINDOWS_H__
#define __GIT_LIST_WINDOWS_H__

// This contains the mechanism that allow the buttons to be added and shown
// while the backup loop is running.
// There is a minimal working example of the technique here :
// https://github.com/LaurentClaessens/ex_qt

#include<string>
#include<QVBoxLayout>
#include<QMainWindow>

#include "GitRepository.h"

using std::string;

class GitListWindow : public QMainWindow
{
    Q_OBJECT;

    private :
        QVBoxLayout* main_layout;
    public:
        GitListWindow();
        void addGitButton(GitRepository);
};

#endif   //__GIT_LIST_WINDOWS_H__
