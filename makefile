lora:tasks.o lora.cpp directoryconverter.o
	LC_ALL=C g++  -std=c++11 -g -rdynamic  lora.cpp  -o lora tasks.o  directoryconverter.o /usr/lib/i386-linux-gnu/libboost_filesystem.so  /usr/lib/i386-linux-gnu/libboost_thread.so -lboost_filesystem -lboost_system -lboost_thread 
tasks.o:tasks.cpp tasks.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o tasks.o -lboost_filesystem -lboost_system  tasks.cpp
directoryconverter.o:directoryconverter.h directoryconverter.cpp
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o directoryconverter.o -lboost_filesystem -lboost_system  directoryconverter.cpp
GitRepository.o: GitRepository.cpp GitRepository.h CommandLine.o
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o GitRepository.o GitRepository.cpp
CommandLine.o:CommandLine.h CommandLine.cpp HashTable.h
	LC_ALL=C g++ -std=c++11  -g -rdynamic -c -o CommandLine.o -lboost_filesystem -lboost_system  CommandLine.cpp
UnitTests:UnitTests.cpp HashTable.h CommandLine.o
	LC_ALL=C g++  -std=c++11 UnitTests.cpp  -o UnitTests CommandLine.o   /usr/lib/i386-linux-gnu/libboost_filesystem.so -lboost_filesystem -lboost_system 
clean:
	rm *.o
