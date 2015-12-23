#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__


#include<string>
#include<QDialog>
#include<QHBoxLayout>
#include<QPushButton>

QPushButton* new_button(std::string);

class MainWindows : public QDialog
{
    Q_OBJECT;

    private :
        QVBoxLayout* main_layout=new QVBoxLayout;
    public:
        MainWindows(QWidget* parent=0);
        void addButton(std::string);
};

#endif   //__MAIN_WINDOWS_H__
