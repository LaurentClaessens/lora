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



// 'newbaka' has a loop over all the files/directories to be backuped and creates a list of taskes ('task_list'). That list is read by a thread which lanch the 'run' method over the elements of the task list.
// There are the following types of taskes :
// - FileCopyTask describe the operations to backup a file
// - DirectoryCopyTask describe the operations to backup a directory 
// - FinalTask is a dummy task that only says to the calling thread that we are done.
// all are derivated from GenericTask (in order to fool the type control and put more than one type in the same list).
//
// The return value of 'run' (boolean) says if one has to continue or bot with the next task. In a normal execution the only task returning 'false' is the final one created right after the main loop.


class GenericTask{
    public:
        GenericTask(){ }
        virtual bool run(){ throw string("You tried to run a GenericTask"); }
};

// FileCopyTask do
// - move the backup file (if exists) to the purge repertory
// - copy the home file to the backup repertory.
// The main loop (especially DealWith...) determines the paths : this run() is stupid.
// The last_write_time attribute of the copied file is the one of the original file.
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

class FinalTask : public GenericTask{
    public:
        EndingTask(){ }
    bool run()
    {
        return false;
    }
};
