# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/didi/opensource/orangeLab/vodka/clion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/vodka_runtime.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vodka_runtime.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vodka_runtime.dir/flags.make

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.o: CMakeFiles/vodka_runtime.dir/flags.make
CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.o: /Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.o -c /Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp > CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.i

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.s

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.o: CMakeFiles/vodka_runtime.dir/flags.make
CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.o: /Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.o -c /Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp > CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.i

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.s

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.o: CMakeFiles/vodka_runtime.dir/flags.make
CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.o: /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.o -c /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp > CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.i

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.s

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.o: CMakeFiles/vodka_runtime.dir/flags.make
CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.o: /Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.o -c /Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp > CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.i

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.s

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.o: CMakeFiles/vodka_runtime.dir/flags.make
CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.o: /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.o -c /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp > CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.i

CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp -o CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.s

vodka_runtime: CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/context.cpp.o
vodka_runtime: CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/timer.cpp.o
vodka_runtime: CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer.cpp.o
vodka_runtime: CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/runtime.cpp.o
vodka_runtime: CMakeFiles/vodka_runtime.dir/Users/didi/opensource/orangeLab/vodka/src/runtime/dom_timer_coordinator.cpp.o
vodka_runtime: CMakeFiles/vodka_runtime.dir/build.make

.PHONY : vodka_runtime

# Rule to build all files generated by this target.
CMakeFiles/vodka_runtime.dir/build: vodka_runtime

.PHONY : CMakeFiles/vodka_runtime.dir/build

CMakeFiles/vodka_runtime.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vodka_runtime.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vodka_runtime.dir/clean

CMakeFiles/vodka_runtime.dir/depend:
	cd /Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/didi/opensource/orangeLab/vodka/clion /Users/didi/opensource/orangeLab/vodka/clion /Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug /Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug /Users/didi/opensource/orangeLab/vodka/clion/cmake-build-debug/CMakeFiles/vodka_runtime.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vodka_runtime.dir/depend
