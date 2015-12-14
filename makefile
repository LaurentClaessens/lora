#############################################################################
# Makefile for building: lora
# Generated by qmake (2.01a) (Qt 4.8.6) on: dim. d?c. 13 08:25:29 2015
# Project:  lora.pro
# Template: app
# Command: /usr/lib/i386-linux-gnu/qt4/bin/qmake -o Makefile lora.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = LC_ALL=C g++ -std=c++11
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
BOOST_SYSTEM  = -lboost_filesystem -lboost_system 
BOOST_THREAD  = -lboost_thread
BOOST_THREAD_LIB  = /usr/lib/i386-linux-gnu/libboost_thread.so 
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/i386-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = CommandLine.cpp \
		directoryconverter.cpp \
		GitRepository.cpp \
		GitWindows.cpp \
		lora.cpp \
		tasks.cpp \
		testing.cpp \
		UnitTests.cpp 
OBJECTS       = CommandLine.o \
		directoryconverter.o \
		GitRepository.o \
		GitWindows.o \
		lora.o \
		tasks.o \
		testing.o \
		UnitTests.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		lora.pro
QMAKE_TARGET  = lora
DESTDIR       = 

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules


Makefile: lora.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile lora.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile lora.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/lora1.0.0 || $(MKDIR) .tmp/lora1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/lora1.0.0/ && $(COPY_FILE) --parents CommandLine.h directoryconverter.h GitRepository.h GitWindows.h HashTable.h lora.h   tasks.h testing.h  .tmp/lora1.0.0/ && $(COPY_FILE) --parents CommandLine.cpp directoryconverter.cpp GitRepository.cpp GitWindows.cpp lora.cpp tasks.cpp testing.cpp UnitTests.cpp .tmp/lora1.0.0/ && (cd `dirname .tmp/lora1.0.0` && $(TAR) lora1.0.0.tar lora1.0.0 && $(COMPRESS) lora1.0.0.tar) && $(MOVE) `dirname .tmp/lora1.0.0`/lora1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/lora1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile
lora: lora.cpp lora.h \
		directoryconverter.o tasks.o MainLoop.o
	$(CXX)  $(CXXFLAGS) $(INCPATH) -o lora MainLoop.o  directoryconverter.o tasks.o   $(BOOST_SYSTEM)  $(BOOST_THREAD)  lora.cpp  $(BOOST_THREAD_LIB)

MainLoop.o: MainLoop.cpp MainLoop.h Configuration.o
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MainLoop.o  MainLoop.cpp

Configuration.o: Configuration.cpp Configuration.h tasks.o
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Configuration.o  Configuration.cpp

tasks.o: tasks.cpp tasks.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tasks.o $(BOOST_SYSTEM)   tasks.cpp

CommandLine.o: CommandLine.cpp CommandLine.h \
		HashTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CommandLine.o $(BOOST_SYSTEM) CommandLine.cpp

directoryconverter.o: directoryconverter.cpp directoryconverter.h \
		tasks.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o directoryconverter.o directoryconverter.cpp

GitRepository.o: GitRepository.cpp GitRepository.h \
		CommandLine.h \
		HashTable.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GitRepository.o  -lboost_filesystem -lboost_system   GitRepository.cpp

GitWindows.o: GitWindows.cpp GitWindows.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GitWindows.o  $(BOOST_SYSTEM)    GitWindows.cpp


testing.o: testing.cpp testing.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o testing.o testing.cpp

UnitTests: UnitTests.cpp testing.h CommandLine.h HashTable.h GitRepository.h \
	GitRepository.o 	testing.o CommandLine.o GitWindows.o
	$(CXX) $(LFLAGS)  $(CXXFLAGS) $(INCPATH) -o UnitTests GitRepository.o  testing.o $(BOOST_SYSTEM) CommandLine.o GitWindows.o UnitTests.cpp  $(LIBS) 

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

