# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kouga/Documents/cpp/PakuVim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kouga/Documents/cpp/PakuVim/build

# Utility rule file for paku_autogen.

# Include the progress variables for this target.
include CMakeFiles/paku_autogen.dir/progress.make

CMakeFiles/paku_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target paku"
	/usr/bin/cmake -E cmake_autogen /home/kouga/Documents/cpp/PakuVim/build/CMakeFiles/paku_autogen.dir ""

paku_autogen: CMakeFiles/paku_autogen
paku_autogen: CMakeFiles/paku_autogen.dir/build.make

.PHONY : paku_autogen

# Rule to build all files generated by this target.
CMakeFiles/paku_autogen.dir/build: paku_autogen

.PHONY : CMakeFiles/paku_autogen.dir/build

CMakeFiles/paku_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/paku_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/paku_autogen.dir/clean

CMakeFiles/paku_autogen.dir/depend:
	cd /home/kouga/Documents/cpp/PakuVim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build/CMakeFiles/paku_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/paku_autogen.dir/depend

