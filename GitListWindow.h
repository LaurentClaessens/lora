#ifndef __GIT_LIST_WINDOWS_H__
#define __GIT_LIST_WINDOWS_H__

// This contains the mechanism that allow the buttons to be added and shown
// while the backup loop is running.
// There is a minimal working example of the technique here :
// https://github.com/LaurentClaessens/ex_qt

#include<string>
#include<QVBoxLayout>
#include<QMainWindow>
#include<QPushButton>

#include "GitRepository.h"

using std::string;

class GitButton : public QPushButton
{
    Q_OBJECT

    private slots:
        void launchGitWindow();
    private:
        GitRepository repo;
    public:
        GitButton(GitRepository);
};

// This is the main window. It contains the list of buttons named after
// the directories that are git repositories needing some intervention.
class GitListWindow : public QMainWindow
{
    Q_OBJECT;

    signals:
        void need_git_window();
    private :
        QVBoxLayout* main_layout;
    public:
        GitListWindow();
        void addGitButton(GitRepository);
};

#endif   //__GIT_LIST_WINDOWS_H__
