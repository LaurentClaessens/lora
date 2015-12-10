#include  <iostream>
#include <string>

#include "../CommandLine.h"
using namespace std;

int main()
{
    CommandLine cl=CommandLine("cat lora.cpp");
    cl.setEnvironmentVariable("LC_ALL","C");
    cl.setEnvironmentVariable("BLA","foo");
    cl.setWorkingDirectory("..");
    string s=cl.toString();
    cout<<s<<endl;
    assert(s=="LC_ALL=C BLA=foo");
}

