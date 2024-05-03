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

#ifndef  __UNIT_TESTS__H
#define __UNIT_TESTS__H


#include <iostream>
#include <string>
#include <functional>

// This is only because I did not RTFM.
// http://cppunit.sourceforge.net/doc/lastest/cppunit_cookbook.html

void test_assert(bool,std::string);

class TestException : public std::exception
{
    private:
        std::string message;
    public:
        TestException(std::string);
        const char* what();
};

// The testing functions have to take no arguments and return void.
class GenericTestingFunction
{
    private:
        std::string name;
        std::string question;
        bool interactive=false;
        std::function<void()> test;
    public :
        GenericTestingFunction (const std::string name ,const bool interactive, std::function<void()>,std::string question="" );
        void run();
};

#endif //__UNIT_TESTS__H
