
bool DirectoryMoveTask::run() const{

    if (!is_directory(orig_path))
    {
        throw string( "The directory "+orig_path.string()+" has to be moved but does not exists anymore ..." );
    }
    assert( !is_directory(destination_path) );

    create_directory_tree(destination_path);
    rename(orig_path,destination_path);

    assert( !is_directory(orig_path) );
    assert( is_directory(destination_path) );
    return true;
}

