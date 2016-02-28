void test_ht3() {
  HashTable<int,string> my_hash; 
  my_hash[1]="hello 1";
  my_hash[2]="hello 2";
  my_hash[3]="hello 3";
  assert(my_hash[2]=="hello 2");

  my_hash[2]="re-hello 2";      // We can overwrite a value by "calling it"
  assert(my_hash[2]=="re-hello 2");
}
