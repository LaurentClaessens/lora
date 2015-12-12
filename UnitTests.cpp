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



#include  <iostream>
#include  <fstream>
#include <string>

#include "CommandLine.h"
#include "HashTable.h"
using namespace std;
    
// A simple command line :
void test_cl1()
{
    CommandLine cl1=CommandLine("cat lora.cpp");
    cl1.setEnvironmentVariable("LC_ALL","C");
    cl1.setEnvironmentVariable("BLA","foo");
    cl1.setWorkingDirectory("..");
    string s=cl1.toString();
    assert(s=="cd ..&& LC_ALL=C BLA=foo cat lora.cpp");
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
    assert(out2==sfile);
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
    
    assert(s1=="hello 4hello 5hello 1");
    // iteration with ++itr. The result is the same
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();++itr) {s2=s2+my_hash[itr]; }
    assert(s1==s2);
}

// We can change the value of a key-value pair.
void test_ht2()
{
    HashTable<int,string> my_hash;
    my_hash.setValue(1,"hello 1");
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(4,"re-hello 4");
    assert(my_hash[4]=="re-hello 4");
}

// Assigning by "calling"      This is the main ressemblance with the Python's dictionary
// We can initiate a HashTable by assigning the values with []
void test_ht3()
{
    HashTable<int,string> my_hash;
    my_hash[1]="hello 1";
    my_hash[2]="hello 2";
    my_hash[3]="hello 3";
    assert(my_hash[1]=="hello 1");
    assert(my_hash[2]=="hello 2");
    assert(my_hash[3]=="hello 3");

    // We can overwrite a value by "calling it"
    my_hash[2]="re-hello 2";
    assert(my_hash[2]=="re-hello 2");

    // We can create a new key-value pair by "calling" it
    my_hash[6]="hello 6";
    assert(my_hash[6]=="hello 6");
}

// a <string,float> example.
void test_ht4()
{
    HashTable<string,float> my_hash;
    my_hash["foo"]=5.4;
    assert( my_hash["foo"]==float(5.4) );       
            // The funny think with rounding is that the following does not work :
            // assert( my_hash["foo"]==5.4 );       
}

TestException::TestException(string m):message(m){}
const char* TestException::what {return message;}

GenericTestingFunction::GenericTestingFunction(string n,bool b,string q):name(n),interavtive(b)
{
    if (interactive)
    {
        if (q=="")
        {
            throws string("When you are defining an interactive test, you have to pass a question.")
        }
        question=q;
    }
    // If not interactive, the question is useless.
}

GenericTestingFunction::run()
{
    bool do_it=false;
    if (interactive)
    {
        string yn="n";
        std::cout<<question+" (y/n)";
        std::cin>>yn;
        if (yn=="y") { do_it=true; }
    }
    else { do_it=true; }
    if (do_it)
    {
        try
        {
            test();
            std::cout<<"Ok for test "+name;
        }
        catch (TestException t){ std::cout<<t.message<<std::endl;  }
    }
    else
    {
        cout<<"Not done (no interactive mode) : "+name;
    }
}

int main(int argc, char *argv[])
{
    bool interactive=false;
    if (argc==2)
    {
        if (argv[1]=="-y"){ interactive=true; }
    }

    GenericTestingFunction("test_cl1",false,test_cl1).run();
    GenericTestingFunction("test_cl2",false,test_cl2).run();
    GenericTestingFunction("test_cl3",true,test_cl3,"See Vim in a new terminal ?").run();

    GenericTestingFunction("test_ht1",false,test_ht1).run();
    GenericTestingFunction("test_ht2",false,test_ht2).run();
    GenericTestingFunction("test_ht3",false,test_ht3).run();
    GenericTestingFunction("test_ht4",false,test_ht5).run();
}
