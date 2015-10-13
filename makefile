newbaka:tasks newbaka.cpp
	LC_ALL=C g++  -o newbaka tasks.o /usr/lib/i386-linux-gnu/libboost_filesystem.so   /usr/lib/i386-linux-gnu/libboost_thread.so   /usr/lib/i386-linux-gnu/libboost_date_time.so    -lboost_filesystem -lboost_system -lboost_thread newbaka.cpp 

tasks:tasks.cpp
	LC_ALL=C g++ -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp
