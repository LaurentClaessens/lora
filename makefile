
####### Compiler, tools and options

CXX           = LC_ALL=C g++ -std=c++11    #c++11 for  std::to_string
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)

BASE_DIR      = ${CURDIR}
BOOST_DIR     = $(BASE_DIR)/boost/boost_1_80_0
INC_BOOST     = -I $(BOOST_DIR)

BOOST_LIB_DIR = $(BOOST_DIR)/stage/lib
BOOST_SYSTEM  = $(BOOST_LIB_DIR)/libboost_filesystem.a
BOOST_THREAD  = $(BOOST_LIB_DIR)/libboost_thread.a

#BOOST_SYSTEM  = -lboost_filesystem -lboost_system 
#BOOST_THREAD  = -lboost_thread
#BOOST_THREAD_LIB  = /usr/lib/i386-linux-gnu/libboost_thread.so 
#BOOST_THREAD_LIB  = /usr/lib/x86_64-linux-gnu/libboost_thread.so 

LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu  -lpthread 
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu  -lpthread 
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f

####### Implicit rules

clean: 
	-$(DEL_FILE) *.o
	-$(DEL_FILE) *~ core *.core


####### Compile
all:  lora UnitTests 
lora: lora.cpp  \
		DirectoryConverter.o tasks.o MainLoop.o Configuration.o   Logging.o
	$(CXX)  $(CXXFLAGS) $(INCPATH) -o lora MainLoop.o Logging.o   Configuration.o DirectoryConverter.o tasks.o   $(BOOST_SYSTEM)  $(BOOST_THREAD)  lora.cpp  $(BOOST_THREAD_LIB) $(LIBS)
UnitTests: UnitTests.cpp\
   	testing.o  HashTable.o  Configuration.o DirectoryConverter.o tasks.o
	$(CXX) $(LFLAGS)  $(CXXFLAGS) $(INCPATH) -o UnitTests  Logging.o Configuration.o testing.o DirectoryConverter.o tasks.o    $(BOOST_SYSTEM) UnitTests.cpp  $(LIBS) 

MainLoop.o: MainLoop.cpp MainLoop.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MainLoop.o MainLoop.cpp

Configuration.o: Configuration.cpp Configuration.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Configuration.o  Configuration.cpp

Logging.o: Logging.cpp Logging.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Logging.o  Logging.cpp

tasks.o: tasks.cpp tasks.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tasks.o $(BOOST_SYSTEM)   tasks.cpp

DirectoryConverter.o: DirectoryConverter.cpp DirectoryConverter.h \
		tasks.h Configuration.o
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DirectoryConverter.o DirectoryConverter.cpp

testing.o: testing.cpp testing.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o testing.o testing.cpp
HashTable.o:  HashTable.h			
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o HashTable.o HashTable.h

