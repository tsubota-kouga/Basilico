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
include CMakeFiles/paku.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/paku.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/paku.dir/flags.make

CMakeFiles/paku.dir/src/main.cpp.o: CMakeFiles/paku.dir/flags.make
CMakeFiles/paku.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/paku.dir/src/main.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/paku.dir/src/main.cpp.o -c /home/kouga/Documents/cpp/PakuVim/src/main.cpp

CMakeFiles/paku.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paku.dir/src/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kouga/Documents/cpp/PakuVim/src/main.cpp > CMakeFiles/paku.dir/src/main.cpp.i

CMakeFiles/paku.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paku.dir/src/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kouga/Documents/cpp/PakuVim/src/main.cpp -o CMakeFiles/paku.dir/src/main.cpp.s

CMakeFiles/paku.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/paku.dir/src/main.cpp.o.requires

CMakeFiles/paku.dir/src/main.cpp.o.provides: CMakeFiles/paku.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/paku.dir/build.make CMakeFiles/paku.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/paku.dir/src/main.cpp.o.provides

CMakeFiles/paku.dir/src/main.cpp.o.provides.build: CMakeFiles/paku.dir/src/main.cpp.o


CMakeFiles/paku.dir/src/PakuVim.cpp.o: CMakeFiles/paku.dir/flags.make
CMakeFiles/paku.dir/src/PakuVim.cpp.o: ../src/PakuVim.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/paku.dir/src/PakuVim.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/paku.dir/src/PakuVim.cpp.o -c /home/kouga/Documents/cpp/PakuVim/src/PakuVim.cpp

CMakeFiles/paku.dir/src/PakuVim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paku.dir/src/PakuVim.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kouga/Documents/cpp/PakuVim/src/PakuVim.cpp > CMakeFiles/paku.dir/src/PakuVim.cpp.i

CMakeFiles/paku.dir/src/PakuVim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paku.dir/src/PakuVim.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kouga/Documents/cpp/PakuVim/src/PakuVim.cpp -o CMakeFiles/paku.dir/src/PakuVim.cpp.s

CMakeFiles/paku.dir/src/PakuVim.cpp.o.requires:

.PHONY : CMakeFiles/paku.dir/src/PakuVim.cpp.o.requires

CMakeFiles/paku.dir/src/PakuVim.cpp.o.provides: CMakeFiles/paku.dir/src/PakuVim.cpp.o.requires
	$(MAKE) -f CMakeFiles/paku.dir/build.make CMakeFiles/paku.dir/src/PakuVim.cpp.o.provides.build
.PHONY : CMakeFiles/paku.dir/src/PakuVim.cpp.o.provides

CMakeFiles/paku.dir/src/PakuVim.cpp.o.provides.build: CMakeFiles/paku.dir/src/PakuVim.cpp.o


CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o: CMakeFiles/paku.dir/flags.make
CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o: paku_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o -c /home/kouga/Documents/cpp/PakuVim/build/paku_autogen/mocs_compilation.cpp

CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kouga/Documents/cpp/PakuVim/build/paku_autogen/mocs_compilation.cpp > CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.i

CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kouga/Documents/cpp/PakuVim/build/paku_autogen/mocs_compilation.cpp -o CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.s

CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.requires:

.PHONY : CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.requires

CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.provides: CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.requires
	$(MAKE) -f CMakeFiles/paku.dir/build.make CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.provides.build
.PHONY : CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.provides

CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.provides.build: CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o


# Object files for target paku
paku_OBJECTS = \
"CMakeFiles/paku.dir/src/main.cpp.o" \
"CMakeFiles/paku.dir/src/PakuVim.cpp.o" \
"CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o"

# External object files for target paku
paku_EXTERNAL_OBJECTS =

paku: CMakeFiles/paku.dir/src/main.cpp.o
paku: CMakeFiles/paku.dir/src/PakuVim.cpp.o
paku: CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o
paku: CMakeFiles/paku.dir/build.make
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5WebEngineWidgets.so.5.11.1
paku: ../neovim.cpp/build/libnvim_client.so
paku: /usr/lib/x86_64-linux-gnu/libboost_system.so
paku: /usr/lib/x86_64-linux-gnu/libboost_coroutine.so
paku: /usr/lib/x86_64-linux-gnu/libboost_context.so
paku: /usr/lib/x86_64-linux-gnu/libboost_thread.so
paku: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
paku: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
paku: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
paku: /usr/lib/x86_64-linux-gnu/libpthread.so
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5WebEngineCore.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5WebChannel.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Positioning.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Quick.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Qml.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5PrintSupport.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Widgets.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Gui.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Network.so.5.11.1
paku: /home/kouga/Qt/5.11.1/gcc_64/lib/libQt5Core.so.5.11.1
paku: CMakeFiles/paku.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kouga/Documents/cpp/PakuVim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable paku"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paku.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/paku.dir/build: paku

.PHONY : CMakeFiles/paku.dir/build

CMakeFiles/paku.dir/requires: CMakeFiles/paku.dir/src/main.cpp.o.requires
CMakeFiles/paku.dir/requires: CMakeFiles/paku.dir/src/PakuVim.cpp.o.requires
CMakeFiles/paku.dir/requires: CMakeFiles/paku.dir/paku_autogen/mocs_compilation.cpp.o.requires

.PHONY : CMakeFiles/paku.dir/requires

CMakeFiles/paku.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/paku.dir/cmake_clean.cmake
.PHONY : CMakeFiles/paku.dir/clean

CMakeFiles/paku.dir/depend:
	cd /home/kouga/Documents/cpp/PakuVim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build /home/kouga/Documents/cpp/PakuVim/build/CMakeFiles/paku.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/paku.dir/depend

