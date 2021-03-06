CC =  /usr/bin/clang
CXX = /usr/bin/clang++
CXXOUTNAME = macClangProfiler
CXXFLAGS = -std=c++11 -Wall 
CXXFLAGS += 
CXXOPT += -O2 -DOPTLEVEL=\"O2\" -funroll-loops -DUNROLL_LOOPS -DNDEBUG 
ifneq ($(shell uname -s),Darwin)
	CXXOPT += -march=native -mtune=native 
endif
LD_FLAGS += 
#debug
CXXDEBUG = -g -gstabs+ 
INSTALL_DIR=tempInstallTest

USE_CPPITERTOOLS = 1
USE_CPPPROGUTILS = 1
