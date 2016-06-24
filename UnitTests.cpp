/*
Copyright 2015 Laurent Claessens
contact : laurent@claessens-donadello.eu

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

#include "testing.h"
#include "Configuration.h"
#include "HashTable.h"

using namespace std;

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
    // iteration with ++itr. The result is the same.
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();++itr) {s2=s2+my_hash[itr]; }
    test_assert(s1==s2,"loop in HashTable with ++itr");
    string s3="";

    // We can get the value and the key from the iterator.
    HashTable<string,string> my_other_hash;
    my_other_hash["Kone"]="Vone";
    my_other_hash["Ktwo"]="Vtwo";
    my_other_hash["Kthree"]="Vthree";
    for (auto itr=my_other_hash.begin();itr!=my_other_hash.end();++itr) 
    {
        s3=s3+itr->key;
        s3=s3+itr->value;
    }
    test_assert(s3=="KoneVoneKtwoVtwoKthreeVthree","Value and key retrieving from an iterator");
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

void test_ht5()
{
    HashTable<string,int> my_hash;
    my_hash["foo"]=1;
    my_hash["bar"]=2;
    my_hash["egg"]=3;
    my_hash.reset();
    test_assert(my_hash.isEmpty()==1,"the reset function does not empty the hash table.");
}

void test_exclude()
{
    Configuration* config_ptr=configuration_file_to_configuration("example.cfg","",false);   // the last boolean argument is 'verbose'
    test_assert(config_ptr->is_excluded("/home/daniel/stuff")==true,"This should be excluded.");
    test_assert(config_ptr->is_excluded("/home/daniel/stuff/")==true,"This should be excluded.");
    test_assert(config_ptr->is_excluded("/home/daniel/foo/bar")==true,"This should be excluded.");
    test_assert(config_ptr->is_excluded("/home/daniel/foo/bar/")==true,"This should be excluded.");
}

int main()
{
    std::cout<<"Initializing tests ---------------------------"<<std::endl<<std::endl<<std::endl;
    GenericTestingFunction("test_exclude",false,test_exclude).run();

    GenericTestingFunction("test_ht1",false,test_ht1).run();
    GenericTestingFunction("test_ht2",false,test_ht2).run();
    GenericTestingFunction("test_ht3",false,test_ht3).run();
    GenericTestingFunction("test_ht4",false,test_ht4).run();
    GenericTestingFunction("test_ht5",false,test_ht5).run();
}
