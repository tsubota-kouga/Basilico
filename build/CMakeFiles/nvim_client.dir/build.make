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

# Include any dependencies generated for this target.
include CMakeFiles/nvim_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/nvim_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nvim_client.dir/flags.make

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o: CMakeFiles/nvim_client.dir/flags.make
CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o: ../neovim.cpp/src/neovim.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o -c /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/neovim.cpp

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.i: cmake_force
	@echo "Preprocessing CXX source to CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/neovim.cpp > CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.i

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.s: cmake_force
	@echo "Compiling CXX source to assembly CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/neovim.cpp -o CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.s

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.requires:

.PHONY : CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.requires

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.provides: CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.requires
	$(MAKE) -f CMakeFiles/nvim_client.dir/build.make CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.provides.build
.PHONY : CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.provides

CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.provides.build: CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o


CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o: CMakeFiles/nvim_client.dir/flags.make
CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o: ../neovim.cpp/src/socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o -c /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/socket.cpp

CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.i: cmake_force
	@echo "Preprocessing CXX source to CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/socket.cpp > CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.i

CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.s: cmake_force
	@echo "Compiling CXX source to assembly CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kouga/Documents/cpp/PakuVim/neovim.cpp/src/socket.cpp -o CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.s

CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.requires:

.PHONY : CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.requires

CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.provides: CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/nvim_client.dir/build.make CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.provides.build
.PHONY : CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.provides

CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.provides.build: CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o


# Object files for target nvim_client
nvim_client_OBJECTS = \
"CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o" \
"CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o"

# External object files for target nvim_client
nvim_client_EXTERNAL_OBJECTS =

libnvim_client.so: CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o
libnvim_client.so: CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o
libnvim_client.so: CMakeFiles/nvim_client.dir/build.make
libnvim_client.so: CMakeFiles/nvim_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libnvim_client.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nvim_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nvim_client.dir/build: libnvim_client.so

.PHONY : CMakeFiles/nvim_client.dir/build

CMakeFiles/nvim_client.dir/requires: CMakeFiles/nvim_client.dir/neovim.cpp/src/neovim.cpp.o.requires
CMakeFiles/nvim_client.dir/requires: CMakeFiles/nvim_client.dir/neovim.cpp/src/socket.cpp.o.requires

.PHONY : CMakeFiles/nvim_client.dir/requires

CMakeFiles/nvim_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nvim_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nvim_client.dir/clean

CMakeFiles/nvim_client.dir/depend:
	cd /home/kouga/Documents/cpp/PakuVim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build/CMakeFiles/nvim_client.dir/DependInfo.cmake
.PHONY : CMakeFiles/nvim_client.dir/depend

