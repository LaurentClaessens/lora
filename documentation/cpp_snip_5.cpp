void loops(Configuration* config_ptr)
{
    HashTable<string,MainLoop*> loops_table;
    loops_table["backup loop"]=new MainBackupLoop(config_ptr);
    loops_table["purge loop"]=new MainPurgeLoop(config_ptr);

    loops_table["backup loop"]->run();
    loops_table["purge loop"]->run();
}

