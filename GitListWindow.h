#ifndef __GIT_LIST_WINDOW_H__
#define __GIT_LIST_WINDOW_H__

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

class Configuration;

// This is the main window. It contains the list of buttons named after
// the directories that are git repositories needing some intervention.
class GitListWindow : public QMainWindow
{
    Q_OBJECT;

    signals:
        void need_git_window();
    public slots:
        void finished();
    private :
        QVBoxLayout* main_layout;
        const Configuration* config_ptr;
        bool is_finished;
    public:
        GitListWindow(const Configuration*);
        void launchGitWindow(GitRepository);
        void addGitButton(GitRepository);
        void processEvents();
        void join();
};

// When asked to launch a git window, the button refers to his parent
// in order to pass the Configuration* pointer.
class GitButton : public QPushButton
{
    Q_OBJECT

    private slots:
        void launchGitWindow();
    private:
        GitRepository repo;
        GitListWindow* parent;
    public:
        GitButton(GitRepository,GitListWindow*);
};

#endif   //__GIT_LIST_WINDOW_H__
