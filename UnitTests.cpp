
#include  <iostream>
#include <string>

#include "CommandLine.h"
#include "HashTable.h"
using namespace std;

int main()
{
    CommandLine cl=CommandLine("cat lora.cpp");
    cout<<"Initialisé"<<endl;
    cl.setEnvironmentVariable("LC_ALL","C");
    cout<<"un"<<endl;
    cl.setEnvironmentVariable("BLA","foo");
    cout<<"deux"<<endl;
    cl.setWorkingDirectory("..");
    cout<<"c'est parti pour toString"<<endl;
    string s=cl.toString();
    cout<<"s : "<<s<<endl;
    assert(s=="LC_ALL=C BLA=foo cd ..&& cat lora.cpp");


    HashTable<int,string> my_hash;
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(5,"hello 5");
    my_hash.setValue(1,"hello 1");
    try
    {
        my_hash.setValue(4,"this replaces the 'hello 4'");
    }
    catch (string s) 
    { 
        assert( s=="You cannot assign a new value to a key");
    }

    string s1="";
    string s2="";
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();itr++) 
    {
        s1=s1+my_hash[itr]; 
    }

    assert(s1=="hello 4hello 5hello 1");
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();++itr) {s2=s2+my_hash[itr]; }
    assert(s1==s2);

    assert(my_hash[1]=="hello 1");
    assert(my_hash[5]=="hello 5");
    assert(my_hash[4]=="hello 4");
}
