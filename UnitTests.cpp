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

#include <iostream>
#include <fstream>
#include <string>


#include <QApplication>
#include <QPushButton>

#include "testing.h"
#include "CommandLine.h"
#include "HashTable.h"
#include "GitRepository.h"
#include "GitWindows.h"

using namespace std;
    
// A simple command line :
void test_cl1()
{
    CommandLine cl1=CommandLine("cat lora.cpp");
    cl1.setEnvironmentVariable("LC_ALL","C");
    cl1.setEnvironmentVariable("BLA","foo");
    cl1.setWorkingDirectory("..");
    string s=cl1.toString();
    test_assert(s=="cd ..&& LC_ALL=C BLA=foo cat lora.cpp","Command line composition");
}

// This one uses "cat" to read a file and compare to the content of the file
void test_cl2()
{
    CommandLine cl2=CommandLine("cat UnitTests.cpp");
    cl2.setEnvironmentVariable("LC_ALL","C");
    string out2=cl2.getOutput();

    ifstream file;
    file.open("UnitTests.cpp");
    if (!file)
    {
        cerr << "The file on which I was about to test is not available";
        exit(1);   // call system to stop
    }
    string sfile;
    string x;
    while (std::getline(file,x)) 
    {
        sfile = sfile + x+"\n";
    }
    test_assert(out2==sfile,"get the output of 'cat UnitTests.cpp'");
}

// Launching Vim in a terminal
void test_cl3()
{
    CommandLine cl=CommandLine("vim");
    cl.setWorkingDirectory("..");
    cl.setTerminal("terminology -e");
    cl.run();
}

// Iterators with HashTable
void test_ht1()
{
    HashTable<int,string> my_hash;
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(5,"hello 5");
    my_hash.setValue(1,"hello 1");
    
    // iteration with itr++
    string s1="";
    string s2="";
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();itr++) 
    {
       s1=s1+my_hash[itr]; 
    }
    
    test_assert(s1=="hello 4hello 5hello 1","loop in HashTable with itr++");
    // iteration with ++itr. The result is the same
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();++itr) {s2=s2+my_hash[itr]; }
    test_assert(s1==s2,"loop in HashTable with ++itr");
}

// We can change the value of a key-value pair.
void test_ht2()
{
    HashTable<int,string> my_hash;
    my_hash.setValue(1,"hello 1");
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(4,"re-hello 4");
    test_assert(my_hash[4]=="re-hello 4","Assigning a new value to a key in HashTable");
}

// Assigning by "calling"      This is the main ressemblance with the Python's dictionary
// We can initiate a HashTable by assigning the values with []
void test_ht3()
{
    HashTable<int,string> my_hash;
    my_hash[1]="hello 1";
    my_hash[2]="hello 2";
    my_hash[3]="hello 3";
    test_assert(my_hash[1]=="hello 1","Creating a key by 'calling' it");
    test_assert(my_hash[2]=="hello 2","Creating a key by 'calling' it");
    test_assert(my_hash[3]=="hello 3","Creating a key by 'calling' it");

    // We can overwrite a value by "calling it"
    my_hash[2]="re-hello 2";
    test_assert(my_hash[2]=="re-hello 2","Assigning a new value by calling");

    // We can create a new key-value pair by "calling" it
    my_hash[6]="hello 6";
    test_assert(my_hash[6]=="hello 6","Assigning a new value by calling");
}

// a <string,float> example.
void test_ht4()
{
    HashTable<string,float> my_hash;
    my_hash["foo"]=5.4;
    test_assert( my_hash["foo"]==float(5.4),"Small test with <string,float>" );
            // The funny think with rounding is that the following does not work :
            // assert( my_hash["foo"]==5.4 );       
}

void test_gr1()
{
    std::string commit;
    string repo;
    std::cout<<"What directory do you want to see ? ";
    std::cin>>repo;
    GitRepository gr=GitRepository(repo);
    commit=gr.getStatusMessage();
    std::cout<<commit<<std::endl;

    vector<path> untracked_files=gr.getUntrackedFiles();
    std::cout<<"Untracked files :"<<std::endl;
    for (path& p:untracked_files)
    {
        std::cout<<p<<std::endl;
    }
    vector<path> modified_files=gr.getModifiedFiles();
    std::cout<<"Modified files :"<<std::endl;
    for (path& p:modified_files)
    {
        std::cout<<p<<std::endl;
    }
    if (gr.isClean())
    {
        std::cout<<"This repository is CLEAN."<<std::endl;
    }
    else
    {
        std::cout<<"This repository is NOT CLEAN."<<std::endl;
        std::cout<<"Do you want to see the 'git diff' of this repository ?";
        gr.launchGitDiff();
    }
}

void test_gw1()
{
    std::string commit;
    string directory;
    std::cout<<"What directory do you want to see ? ";
    std::cin>>directory;
    std::cout<<"There will be two of them.";
    GitRepository repo=GitRepository(directory);
    
    GitWindows gw1=GitWindows(repo);
    gw1.launch();

    GitWindows gw2=GitWindows(repo);
    gw2.launch();
}

int main(int argc,char* argv[])
{
    QApplication app(argc, argv);
    GenericTestingFunction("test_gw1",true,test_gw1,"See the git windows ?").run();
    GenericTestingFunction("test_cl1",false,test_cl1).run();
    GenericTestingFunction("test_cl2",false,test_cl2).run();

    GenericTestingFunction("test_ht1",false,test_ht1).run();
    GenericTestingFunction("test_ht2",false,test_ht2).run();
    GenericTestingFunction("test_ht3",false,test_ht3).run();
    GenericTestingFunction("test_ht4",false,test_ht4).run();

    GenericTestingFunction("test_gr1",true,test_gr1,"See a status message ?").run();
    GenericTestingFunction("test_cl3",true,test_cl3,"See Vim in a new terminal ?").run();
}
