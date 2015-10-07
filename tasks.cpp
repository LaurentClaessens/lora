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

class GenericTask{
    public:
        GenericTask(){ }
        virtual bool run(){ throw string("You tried to run a GenericTask"); }
};

class FileCopyTask : public GenericTask{
    private: 
        path orig_path;
        path bak_path;
        path purge_path;

    public:
    FileCopyTask(pathTriple triple):GenericTask()
    {
        this->orig_path=triple.orig;
        this->bak_path=triple.bak;
        this->purge_path=triple.purge;
    }
    bool run(){
        assert(is_regular_file(orig_path));

        cout<<"(file) Copy  "<<this->orig_path;
        cout<<"--->  "<<this->bak_path<<"..."<<endl;

        if (is_regular_file(bak_path))
        {
            create_tree(purge_path.parent_path());
            rename( bak_path,purge_path );
            assert( is_regular_file(purge_path) );
        }
        my_copy_file(  orig_path,bak_path  );


        vector<path> test_list;
        test_list.push_back( orig_path );
        test_list.push_back( bak_path );
        test_list.push_back( purge_path );

        for (int i=0;i<test_list.size();++i)
        {
            if (!is_regular_file(test_list[i])){ 
                string st="The file"+test_list[i].string()+" has not been created.";
                throw st;  }
        }
        assert( is_regular_file(orig_path) );
        assert( is_regular_file(bak_path) );
        return true;
    }
};

class RepertoryCopyTask : public GenericTask{
    public: 
        path orig_path;
        path bak_path;
        RepertoryCopyTask(path orig_path,path bak_path):GenericTask()
    {
        this->orig_path=orig_path;
        this->bak_path=bak_path;
    }
    bool run()
    {
        cout<<"Copy  "<<this->orig_path<<endl;
        cout<<"--->  "<<this->bak_path<<endl;
        copy_tree(orig_path,bak_path);
        return true;
    }
};

class EndingTask : public GenericTask{
    public:
        bool ending_task;
    EndingTask(){ }
    bool run()
    {
        return false;
    }
};
