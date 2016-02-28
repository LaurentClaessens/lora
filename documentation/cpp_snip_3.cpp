
void run_tasks()
{   
    TaskList* const tl_ptr = getTaskList();
    bool still=true;
    while (still)
    {
        if (tl_ptr->size() != 0) { still=run_next(*tl_ptr); }
    }
    std::cout<<"Work done. Leaving the thread."<<std::endl;
}

