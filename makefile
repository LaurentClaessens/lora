newbaka:tasks.o newbaka.cpp newbaka.h configuration.o
	LC_ALL=C g++  -std=c++11 -g -rdynamic  newbaka.cpp  -o newbaka tasks.o configuration.o /usr/lib/i386-linux-gnu/libboost_filesystem.so  /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system -lboost_thread 

tasks.o:tasks.cpp tasks.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp

configuration.o:configuration.cpp configuration.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o configuration.o    -lboost_filesystem -lboost_system  configuration.cpp
