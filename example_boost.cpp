#include <boost/filesystem.hpp>
#include <iostream>

int main()
{
    boost::filesystem::path directory = ".";
    boost::filesystem::directory_iterator end_itr;
    for( boost::filesystem::directory_iterator itr(directory); itr!=end_itr;++itr  )
    {
        boost::filesystem::path pathname=itr->path();
        std::cout<< pathname << std::endl;
    }
}
