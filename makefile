#compiler and c++ standard variables
COMPILER		= g++
STD			= c++17

# directories to look for .h and .hpp files (preceded by -I parameter)
INCLUDE_DIRS		= .

# compiler parameters for compiling and linking
INCLUDE_OPTIONS		= $(foreach dir, $(INCLUDE_DIRS), -I$(dir))
LINKING_OPTIONS		= -g -std=$(STD) $(INCLUDE_OPTIONS) -Wall -Wextra
COMPILING_OPTIONS	= -c $(LINKING_OPTIONS)

# list of project headers
HEADERS			= $(foreach dir, $(INCLUDE_DIRS), $(wildcard $(dir)/*.hpp))

# these variables define one tester program for each header
TESTERS_SRC		= $(HEADERS:.hpp=_test.cpp)
TESTERS_OBJ		= $(TESTERS_SRC:.cpp=.o)
TESTERS			= $(TESTERS_SRC:.cpp=)

# $< refers to prerequisite and $@ refers to target name

# instructions for creating object files from cpp files
%.o : %.cpp
	$(COMPILER) $(COMPILING_OPTIONS) $< -o $@

# instructions for creating tester programs and running them
%_test : %_test.o %.hpp
	$(COMPILER) $(LINKING_OPTIONS) $< -o $@
	./$@

# this indicates that these rules do not correspond to files
.PHONY : test clean

# instructions on how to clean the project (deleting some stuff)
clean :
	rm -f *.o
	rm -f $(TESTERS)

# this runs the test suite
test : $(TESTERS)
