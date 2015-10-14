newbaka:tasks.o newbaka.cpp newbaka.h
	LC_ALL=C g++  -g -rdynamic  newbaka.cpp  -o newbaka tasks.o /usr/lib/i386-linux-gnu/libboost_filesystem.so   /usr/lib/i386-linux-gnu/libboost_thread.so   /usr/lib/i386-linux-gnu/libboost_date_time.so    -lboost_filesystem -lboost_system -lboost_thread 

tasks.o:tasks.cpp tasks.h
	LC_ALL=C g++  -g -rdynamic -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp



