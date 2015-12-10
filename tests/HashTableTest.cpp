#include  <iostream>
#include <string>

#include "../HashTable.h"
using namespace std;

int main()
{
    HashTable<int,string> my_hash;
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(5,"hello 5");
    my_hash.setValue(1,"hello 1");
    my_hash.setValue(4,"Should not work");

    string s1="";
    string s2="";
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();itr++) {
        s1=s1+my_hash[itr]; 
    }

    assert(s1=="hello 4hello 5hello 1");
    for (HashTable<int,string>::iterator itr=my_hash.begin();itr!=my_hash.end();++itr) {s2=s2+my_hash[itr]; }
    assert(s1==s2);

    assert(my_hash[1]=="hello 1");
    assert(my_hash[5]=="hello 5");
}
