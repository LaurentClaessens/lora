void MainLoop::loopOverDirectory(path directory)
{
    directory_iterator end_itr;
    for(  directory_iterator itr(directory); itr!=end_itr;++itr  )
    {
        path pathname=itr->path();
        if (is_directory( pathname  ))
            { DealWithDirectory(pathname); }
        else if (is_regular_file(pathname)) 
            { DealWithFile(pathname); }
        else
        {
          throw std::string("***  What the hell is "+pathname.string()+" ??? ");
        }
    }
}
