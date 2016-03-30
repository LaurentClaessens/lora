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
#include<string>
#include "testing.h"

void test_assert(bool b,std::string message)
{
    if (!b)
    {
        std::cout<<"********************************* your software is juste agglomerate pieces of bullshit -- it don't even pass this elementary test *************************"<<std::endl;
        throw TestException(message+"\n");
    }
}

TestException::TestException(std::string m):message(m){}
const char* TestException::what() {return (message).c_str();}

GenericTestingFunction::GenericTestingFunction(std::string n,bool b,std::function<void()> t,std::string q):name(n),interactive(b),test(t)
{
    if (interactive)
    {
        if (q=="")
        {
            throw std::string("When you are defining an interactive test, you have to pass a question.");
        }
        question=q;
    }
    // If not interactive, the question is useless.
}

void GenericTestingFunction::run()
{
    bool do_it=true;
    if (interactive)
    {
        do_it=false;
        std::string yn="n";
        std::cout<<question+" (y/default : n)";
        std::cin>>yn;
        if (yn=="y") { do_it=true; }
    }
    if (do_it)
    {
        try
        {
            test();
            std::cout<<"Ok for test "+name<<std::endl;
        }
        catch (TestException t){ std::cout<<t.what();  }
    }
    else
    {
        std::cout<<"Not done (no interactive mode) : "+name<<std::endl;
    }
}

