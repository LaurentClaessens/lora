newbaka:tasks.o newbaka.cpp newbaka.h mainbackuploop.o mainpurgeloop.o directoryconverter.o
	LC_ALL=C g++  -std=c++11 -g -rdynamic  newbaka.cpp  -o newbaka tasks.o mainbackuploop.o mainpurgeloop.o directoryconverter.o /usr/lib/i386-linux-gnu/libboost_filesystem.so  /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system -lboost_thread 

tasks.o:tasks.cpp tasks.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp

mainbackuploop.o:mainbackuploop.cpp mainbackuploop.h directoryconverter.o
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o mainbackuploop.o    -lboost_filesystem -lboost_system  mainbackuploop.cpp

mainpurgeloop.o:mainpurgeloop.cpp mainpurgeloop.h directoryconverter.o
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o mainpurgeloop.o    -lboost_filesystem -lboost_system  mainpurgeloop.cpp
directoryconverter.o:directoryconverter.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o directoryconverter.o -lboost_filesystem -lboost_system  directoryconverter.cpp