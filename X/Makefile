#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
CXXFLAGS	:= -std=c++17 -Wall -Wextra -g -O3

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

ifeq ($(OS),Windows_NT)
MAIN		:= x.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
INCLUDEDIRS += "C:\2-code\asio\asio\include" 
INCLUDEDIRS += "C:\2-code\googletest\googletest\include" 
INCLUDEDIRS += "C:\2-code\glog"
INCLUDEDIRS += "C:\2-code\glog\src"
INCLUDEDIRS += "C:\2-code\gflags\include"
INCLUDEDIRS	+= "C:\2-code\json\include"
LIBDIRS		:= $(LIB)
LIBDIRS		+= "C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\i686-w64-mingw32\lib"
LIBDIRS		+= "C:\2-code\googletest\lib"
LIBDIRS		+= "C:\2-code\glog"
LIBDIRS		+= "C:\2-code\gflags\lib"
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD			:= mkdir
CXXFLAGS	+= -DGLOG_NO_ABBREVIATED_SEVERITIES
else
MAIN		:= x
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

ifeq ($(OS),Windows_NT)
LIBS		+= -lws2_32 -lgdi32 -lwsock32
LIBS		+= -lshlwapi
LIBS		+= -lgflags_static
endif

LIBS		+= -lgtest
LIBS		+= -lglog
LIBS		+= -lpthread


# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# define the C object files 
OBJECTS		:= $(SOURCES:.cpp=.o)

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!