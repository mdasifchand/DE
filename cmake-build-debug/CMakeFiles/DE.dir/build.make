# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/asif/Downloads/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/asif/Downloads/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/asif/CLionProjects/DE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/asif/CLionProjects/DE/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DE.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DE.dir/flags.make

CMakeFiles/DE.dir/main.cpp.o: CMakeFiles/DE.dir/flags.make
CMakeFiles/DE.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asif/CLionProjects/DE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DE.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DE.dir/main.cpp.o -c /home/asif/CLionProjects/DE/main.cpp

CMakeFiles/DE.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DE.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asif/CLionProjects/DE/main.cpp > CMakeFiles/DE.dir/main.cpp.i

CMakeFiles/DE.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DE.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asif/CLionProjects/DE/main.cpp -o CMakeFiles/DE.dir/main.cpp.s

# Object files for target DE
DE_OBJECTS = \
"CMakeFiles/DE.dir/main.cpp.o"

# External object files for target DE
DE_EXTERNAL_OBJECTS =

DE: CMakeFiles/DE.dir/main.cpp.o
DE: CMakeFiles/DE.dir/build.make
DE: CMakeFiles/DE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asif/CLionProjects/DE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DE"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DE.dir/build: DE

.PHONY : CMakeFiles/DE.dir/build

CMakeFiles/DE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DE.dir/clean

CMakeFiles/DE.dir/depend:
	cd /home/asif/CLionProjects/DE/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asif/CLionProjects/DE /home/asif/CLionProjects/DE /home/asif/CLionProjects/DE/cmake-build-debug /home/asif/CLionProjects/DE/cmake-build-debug /home/asif/CLionProjects/DE/cmake-build-debug/CMakeFiles/DE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DE.dir/depend

