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

    public:
        QHBoxLayout* main_layout=new QHBoxLayout;
        MainWindows(QWidget* parent=0);
        void add_button(std::string);
};

#endif   //__MAIN_WINDOWS_H__
