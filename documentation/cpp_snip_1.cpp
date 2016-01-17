Configuration* config_ptr=arguments_to_configuration(argc,argv);          

GitListWindow* git_list_window=new GitListWindow(config_ptr);
git_list_window->show();

boost::thread task_runner( run_tasks, config_ptr );
loops(config_ptr);
        
task_runner.join();
git_list_window->join();
