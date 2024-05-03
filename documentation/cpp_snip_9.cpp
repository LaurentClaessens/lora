void test_ht1() {
    HashTable<int,string> my_hash;
    my_hash.setValue(4,"hello 4");
    my_hash.setValue(5,"hello 5");
    my_hash.setValue(1,"hello 1");
    
    string s1="";
    for (auto itr=my_hash.begin();itr!=my_hash.end();itr++) 
         { s1=s1+my_hash[itr]; }
    assert(s1=="hello 4hello 5hello 1");
}
