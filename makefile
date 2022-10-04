
####### Compiler, tools and options

CXX           = LC_ALL=C g++ -std=c++11    #c++11 for  std::to_string
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT

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
LIBS          = -L/usr/lib/i386-linux-gnu  -lpthread 
LIBS          = -L/usr/lib/x86_64-linux-gnu  -lpthread 
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
		DirectoryConverter.o tasks.o MainLoop.o Configuration.o Logging.o
	$(CXX)  $(CXXFLAGS) lora.cpp -o lora   MainLoop.o Logging.o   Configuration.o DirectoryConverter.o tasks.o  $(INC_BOOST)  $(BOOST_SYSTEM)  $(BOOST_THREAD)  $(LIBS)

MainLoop.o: MainLoop.cpp MainLoop.h 
	$(CXX) -c $(CXXFLAGS)  -o MainLoop.o MainLoop.cpp $(INC_BOOST) $(BOOST_SYSTEM)

Configuration.o: Configuration.cpp Configuration.h
	$(CXX) -c $(CXXFLAGS)  -o Configuration.o  Configuration.cpp $(INC_BOOST) $(BOOST_SYSTEM)

Logging.o: Logging.cpp Logging.h 
	$(CXX) -c $(CXXFLAGS)  -o Logging.o  Logging.cpp

HashTable.o:  HashTable.h			
	$(CXX) -c $(CXXFLAGS)  -o HashTable.o HashTable.h

tasks.o: tasks.cpp tasks.h
	$(CXX) -c $(CXXFLAGS)  tasks.cpp -o tasks.o $(INC_BOOST) $(BOOST_SYSTEM)

DirectoryConverter.o: DirectoryConverter.cpp DirectoryConverter.h \
		tasks.h Configuration.o
	$(CXX) -c $(CXXFLAGS)  -o DirectoryConverter.o DirectoryConverter.cpp $(INC_BOOST) $(BOOST_SYSTEM)

testing.o: testing.cpp testing.h
	$(CXX) -c $(CXXFLAGS)  -o testing.o testing.cpp

UnitTests: UnitTests.cpp\
   	testing.o  HashTable.o  Configuration.o DirectoryConverter.o tasks.o
	$(CXX) $(LFLAGS)  $(CXXFLAGS)  -o UnitTests  Logging.o Configuration.o testing.o DirectoryConverter.o tasks.o    $(BOOST_SYSTEM) UnitTests.cpp  $(LIBS) 
