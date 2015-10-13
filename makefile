LC_ALL=C

all:newbaka

newbaka:tasks newbaka.cpp
	LC_ALL=C g++  -o newbaka tasks.o /usr/lib/i386-linux-gnu/libboost_filesystem.so   /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system  /usr/lib/i386-linux-gnu/libboost_date_time.so newbaka.cpp

tasks:tasks.cpp
	LC_ALL=C g++ -c tasks.cpp -o tasks.o /usr/lib/i386-linux-gnu/libboost_filesystem.so  -lboost_filesystem -lboost_system  newbaka.cpp
