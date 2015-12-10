#include  <iostream>
#include <string>

#include "../HashTable.h"
using namespace std;

int main()
{
    HashTable<int,string> my_hash();
    my_hash.setValue(4,"bonjour");
}
