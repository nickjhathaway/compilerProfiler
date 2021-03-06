CC = clang
CXX = clang++
CXXOUTNAME = clangProfilerOpt1
CXXFLAGS = -std=c++11 -Wall -stdlib=libstdc++
CXXFLAGS += -I/Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/ -I /Users/nickhathaway/source_codes/gccs/gcc_toolchains/include/c++/4.8.3/x86_64-apple-darwin13.2.0/
CXXOPT += -O1 -DOPTLEVEL=\"O1\" -funroll-loops -DUNROLL_LOOPS -DNDEBUG 
ifneq ($(shell uname -s),Darwin)
	CXXOPT += -march=native -mtune=native 
endif
LD_FLAGS += -L /Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/ -Wl,-rpath,/Users/nickhathaway/source_codes/gccs/gcc_toolchains/lib/
#debug
CXXDEBUG = -g -gstabs+ 
INSTALL_DIR=tempInstallTest
USE_CPPITERTOOLS = 1
USE_CPPPROGUTILS = 1

