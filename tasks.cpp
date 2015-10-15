/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This is part of 'newbaka': you can redistribute it and/or modify
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
//
//

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include "tasks.h"

bool create_tree(path rep_path)
{
    path parent_path=rep_path.parent_path();
    if (is_directory( parent_path ) ) {}
    else { create_tree(parent_path);  } 
    create_directory(rep_path); 
}

void my_copy_file(path from_path,path to_path)
{
    assert( is_regular_file(from_path) );
    assert( !is_regular_file(to_path) );

    time_t t_ori=last_write_time(from_path);

    std::cout<<"Copy file "<<from_path<<"--> "<<to_path<<std::endl;

    copy_file(from_path,to_path);
    last_write_time( to_path,t_ori );
    
    if( last_write_time(from_path)!=last_write_time(to_path) )
    {
        throw std::string("The last_write_time did not copy well for "+from_path.string());
    };
}

void copy_tree(path orig_path,path bak_path)
{
    std::cout<<"(rep) Copy "<<orig_path<<" --> "<<bak_path<<std::endl;
    create_directory(bak_path);
    boost::filesystem::directory_iterator end_itr;
    for(  boost::filesystem::directory_iterator itr(orig_path); itr!=end_itr;++itr  )
    {
        path pathname=itr->path();
        path bak_sub=bak_path/pathname.filename();
        if (is_directory(pathname))
        {  
            create_directory(bak_sub);
            copy_tree(pathname,bak_sub);
        }
        else if (is_regular_file(pathname))
        {
            my_copy_file(pathname,bak_sub);
        }
    }
    std::cout<<"done (rep "<<orig_path<<")"<<std::endl;
}

GenericTask::GenericTask(){ };
bool GenericTask::run() const
{ 
    throw std::string("You tried to run a GenericTask"); 
} 


FileCopyTask::FileCopyTask(pathTriple triple):GenericTask()
    {
        this->orig_path=triple.orig;
        this->bak_path=triple.bak;
        this->purge_modified_path=triple.purge;
    }
bool FileCopyTask::run() const
{
        assert(is_regular_file(orig_path));

        if (is_regular_file(bak_path))
        {
            create_tree(purge_modified_path.parent_path());
            rename( bak_path,purge_modified_path );
            assert( is_regular_file(purge_modified_path) );
        }
        my_copy_file(  orig_path,bak_path  );


        std::vector<path> test_list;
        test_list.push_back( orig_path );
        test_list.push_back( bak_path );
        test_list.push_back( purge_modified_path );

        for (int i=0;i<test_list.size();++i)
        {
            if (!is_regular_file(test_list[i])){ 
                std::string st="The file"+test_list[i].string()+" has not been created.";
                throw st;  }
        }
        assert( is_regular_file(orig_path) );
        assert( is_regular_file(bak_path) );
        return true;
}

RepertoryCopyTask::RepertoryCopyTask(path orig_path,path bak_path):GenericTask()
    {
        this->orig_path=orig_path;
        this->bak_path=bak_path;
    }
bool RepertoryCopyTask::run() const
    {
        copy_tree(orig_path,bak_path);
        return true;
    }

FinalTask::FinalTask(){}
bool FinalTask::run() const
    {
        return false;
    }
