# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sam/Documents/gene

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sam/Documents/gene/build

# Include any dependencies generated for this target.
include test/CMakeFiles/gene_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/gene_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/gene_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/gene_test.dir/flags.make

test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o: test/CMakeFiles/gene_test.dir/flags.make
test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o: ../test/gene_algo_test.cc
test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o: test/CMakeFiles/gene_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Documents/gene/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o"
	cd /home/sam/Documents/gene/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o -MF CMakeFiles/gene_test.dir/gene_algo_test.cc.o.d -o CMakeFiles/gene_test.dir/gene_algo_test.cc.o -c /home/sam/Documents/gene/test/gene_algo_test.cc

test/CMakeFiles/gene_test.dir/gene_algo_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gene_test.dir/gene_algo_test.cc.i"
	cd /home/sam/Documents/gene/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Documents/gene/test/gene_algo_test.cc > CMakeFiles/gene_test.dir/gene_algo_test.cc.i

test/CMakeFiles/gene_test.dir/gene_algo_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gene_test.dir/gene_algo_test.cc.s"
	cd /home/sam/Documents/gene/build/test && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Documents/gene/test/gene_algo_test.cc -o CMakeFiles/gene_test.dir/gene_algo_test.cc.s

# Object files for target gene_test
gene_test_OBJECTS = \
"CMakeFiles/gene_test.dir/gene_algo_test.cc.o"

# External object files for target gene_test
gene_test_EXTERNAL_OBJECTS =

test/gene_test: test/CMakeFiles/gene_test.dir/gene_algo_test.cc.o
test/gene_test: test/CMakeFiles/gene_test.dir/build.make
test/gene_test: lib/libgtest_main.a
test/gene_test: src/libgene_algo.a
test/gene_test: lib/libgtest.a
test/gene_test: test/CMakeFiles/gene_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sam/Documents/gene/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gene_test"
	cd /home/sam/Documents/gene/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gene_test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/sam/Documents/gene/build/test && /usr/bin/cmake -D TEST_TARGET=gene_test -D TEST_EXECUTABLE=/home/sam/Documents/gene/build/test/gene_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/sam/Documents/gene/build/test -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=gene_test_TESTS -D CTEST_FILE=/home/sam/Documents/gene/build/test/gene_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/gene_test.dir/build: test/gene_test
.PHONY : test/CMakeFiles/gene_test.dir/build

test/CMakeFiles/gene_test.dir/clean:
	cd /home/sam/Documents/gene/build/test && $(CMAKE_COMMAND) -P CMakeFiles/gene_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/gene_test.dir/clean

test/CMakeFiles/gene_test.dir/depend:
	cd /home/sam/Documents/gene/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sam/Documents/gene /home/sam/Documents/gene/test /home/sam/Documents/gene/build /home/sam/Documents/gene/build/test /home/sam/Documents/gene/build/test/CMakeFiles/gene_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/gene_test.dir/depend

