# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/charles/CLionProjects/Implied_Price_Engine_All

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/charles/CLionProjects/Implied_Price_Engine_All/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/SimulateQuotes.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/SimulateQuotes.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/SimulateQuotes.dir/flags.make

examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o: examples/CMakeFiles/SimulateQuotes.dir/flags.make
examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o: ../examples/SimulateQuotes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/charles/CLionProjects/Implied_Price_Engine_All/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o"
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples && /usr/local/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o -c /home/charles/CLionProjects/Implied_Price_Engine_All/examples/SimulateQuotes.cpp

examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.i"
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples && /usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/charles/CLionProjects/Implied_Price_Engine_All/examples/SimulateQuotes.cpp > CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.i

examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.s"
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples && /usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/charles/CLionProjects/Implied_Price_Engine_All/examples/SimulateQuotes.cpp -o CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.s

# Object files for target SimulateQuotes
SimulateQuotes_OBJECTS = \
"CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o"

# External object files for target SimulateQuotes
SimulateQuotes_EXTERNAL_OBJECTS =

examples/SimulateQuotes: examples/CMakeFiles/SimulateQuotes.dir/SimulateQuotes.cpp.o
examples/SimulateQuotes: examples/CMakeFiles/SimulateQuotes.dir/build.make
examples/SimulateQuotes: include/libIElib.a
examples/SimulateQuotes: examples/CMakeFiles/SimulateQuotes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/charles/CLionProjects/Implied_Price_Engine_All/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SimulateQuotes"
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimulateQuotes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/SimulateQuotes.dir/build: examples/SimulateQuotes

.PHONY : examples/CMakeFiles/SimulateQuotes.dir/build

examples/CMakeFiles/SimulateQuotes.dir/clean:
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/SimulateQuotes.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/SimulateQuotes.dir/clean

examples/CMakeFiles/SimulateQuotes.dir/depend:
	cd /home/charles/CLionProjects/Implied_Price_Engine_All/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/charles/CLionProjects/Implied_Price_Engine_All /home/charles/CLionProjects/Implied_Price_Engine_All/examples /home/charles/CLionProjects/Implied_Price_Engine_All/build /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples /home/charles/CLionProjects/Implied_Price_Engine_All/build/examples/CMakeFiles/SimulateQuotes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/SimulateQuotes.dir/depend
