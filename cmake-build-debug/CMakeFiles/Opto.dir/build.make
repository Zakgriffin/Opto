# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /Users/zakgriffin/CLionProjects/Opto

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zakgriffin/CLionProjects/Opto/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Opto.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Opto.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Opto.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Opto.dir/flags.make

CMakeFiles/Opto.dir/main.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/main.cpp.o: /Users/zakgriffin/CLionProjects/Opto/main.cpp
CMakeFiles/Opto.dir/main.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Opto.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/main.cpp.o -MF CMakeFiles/Opto.dir/main.cpp.o.d -o CMakeFiles/Opto.dir/main.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/main.cpp

CMakeFiles/Opto.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/main.cpp > CMakeFiles/Opto.dir/main.cpp.i

CMakeFiles/Opto.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/main.cpp -o CMakeFiles/Opto.dir/main.cpp.s

CMakeFiles/Opto.dir/object_view.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/object_view.cpp.o: /Users/zakgriffin/CLionProjects/Opto/object_view.cpp
CMakeFiles/Opto.dir/object_view.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Opto.dir/object_view.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/object_view.cpp.o -MF CMakeFiles/Opto.dir/object_view.cpp.o.d -o CMakeFiles/Opto.dir/object_view.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/object_view.cpp

CMakeFiles/Opto.dir/object_view.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/object_view.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/object_view.cpp > CMakeFiles/Opto.dir/object_view.cpp.i

CMakeFiles/Opto.dir/object_view.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/object_view.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/object_view.cpp -o CMakeFiles/Opto.dir/object_view.cpp.s

CMakeFiles/Opto.dir/user_input.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/user_input.cpp.o: /Users/zakgriffin/CLionProjects/Opto/user_input.cpp
CMakeFiles/Opto.dir/user_input.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Opto.dir/user_input.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/user_input.cpp.o -MF CMakeFiles/Opto.dir/user_input.cpp.o.d -o CMakeFiles/Opto.dir/user_input.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/user_input.cpp

CMakeFiles/Opto.dir/user_input.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/user_input.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/user_input.cpp > CMakeFiles/Opto.dir/user_input.cpp.i

CMakeFiles/Opto.dir/user_input.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/user_input.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/user_input.cpp -o CMakeFiles/Opto.dir/user_input.cpp.s

CMakeFiles/Opto.dir/object.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/object.cpp.o: /Users/zakgriffin/CLionProjects/Opto/object.cpp
CMakeFiles/Opto.dir/object.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Opto.dir/object.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/object.cpp.o -MF CMakeFiles/Opto.dir/object.cpp.o.d -o CMakeFiles/Opto.dir/object.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/object.cpp

CMakeFiles/Opto.dir/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/object.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/object.cpp > CMakeFiles/Opto.dir/object.cpp.i

CMakeFiles/Opto.dir/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/object.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/object.cpp -o CMakeFiles/Opto.dir/object.cpp.s

CMakeFiles/Opto.dir/do_then.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/do_then.cpp.o: /Users/zakgriffin/CLionProjects/Opto/do_then.cpp
CMakeFiles/Opto.dir/do_then.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Opto.dir/do_then.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/do_then.cpp.o -MF CMakeFiles/Opto.dir/do_then.cpp.o.d -o CMakeFiles/Opto.dir/do_then.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/do_then.cpp

CMakeFiles/Opto.dir/do_then.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/do_then.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/do_then.cpp > CMakeFiles/Opto.dir/do_then.cpp.i

CMakeFiles/Opto.dir/do_then.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/do_then.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/do_then.cpp -o CMakeFiles/Opto.dir/do_then.cpp.s

CMakeFiles/Opto.dir/event.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/event.cpp.o: /Users/zakgriffin/CLionProjects/Opto/event.cpp
CMakeFiles/Opto.dir/event.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Opto.dir/event.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/event.cpp.o -MF CMakeFiles/Opto.dir/event.cpp.o.d -o CMakeFiles/Opto.dir/event.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/event.cpp

CMakeFiles/Opto.dir/event.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/event.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/event.cpp > CMakeFiles/Opto.dir/event.cpp.i

CMakeFiles/Opto.dir/event.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/event.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/event.cpp -o CMakeFiles/Opto.dir/event.cpp.s

CMakeFiles/Opto.dir/unknown.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/unknown.cpp.o: /Users/zakgriffin/CLionProjects/Opto/unknown.cpp
CMakeFiles/Opto.dir/unknown.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Opto.dir/unknown.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/unknown.cpp.o -MF CMakeFiles/Opto.dir/unknown.cpp.o.d -o CMakeFiles/Opto.dir/unknown.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/unknown.cpp

CMakeFiles/Opto.dir/unknown.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/unknown.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/unknown.cpp > CMakeFiles/Opto.dir/unknown.cpp.i

CMakeFiles/Opto.dir/unknown.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/unknown.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/unknown.cpp -o CMakeFiles/Opto.dir/unknown.cpp.s

CMakeFiles/Opto.dir/type.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/type.cpp.o: /Users/zakgriffin/CLionProjects/Opto/type.cpp
CMakeFiles/Opto.dir/type.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Opto.dir/type.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/type.cpp.o -MF CMakeFiles/Opto.dir/type.cpp.o.d -o CMakeFiles/Opto.dir/type.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/type.cpp

CMakeFiles/Opto.dir/type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/type.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/type.cpp > CMakeFiles/Opto.dir/type.cpp.i

CMakeFiles/Opto.dir/type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/type.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/type.cpp -o CMakeFiles/Opto.dir/type.cpp.s

CMakeFiles/Opto.dir/editable_text.cpp.o: CMakeFiles/Opto.dir/flags.make
CMakeFiles/Opto.dir/editable_text.cpp.o: /Users/zakgriffin/CLionProjects/Opto/editable_text.cpp
CMakeFiles/Opto.dir/editable_text.cpp.o: CMakeFiles/Opto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Opto.dir/editable_text.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Opto.dir/editable_text.cpp.o -MF CMakeFiles/Opto.dir/editable_text.cpp.o.d -o CMakeFiles/Opto.dir/editable_text.cpp.o -c /Users/zakgriffin/CLionProjects/Opto/editable_text.cpp

CMakeFiles/Opto.dir/editable_text.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Opto.dir/editable_text.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zakgriffin/CLionProjects/Opto/editable_text.cpp > CMakeFiles/Opto.dir/editable_text.cpp.i

CMakeFiles/Opto.dir/editable_text.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Opto.dir/editable_text.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zakgriffin/CLionProjects/Opto/editable_text.cpp -o CMakeFiles/Opto.dir/editable_text.cpp.s

# Object files for target Opto
Opto_OBJECTS = \
"CMakeFiles/Opto.dir/main.cpp.o" \
"CMakeFiles/Opto.dir/object_view.cpp.o" \
"CMakeFiles/Opto.dir/user_input.cpp.o" \
"CMakeFiles/Opto.dir/object.cpp.o" \
"CMakeFiles/Opto.dir/do_then.cpp.o" \
"CMakeFiles/Opto.dir/event.cpp.o" \
"CMakeFiles/Opto.dir/unknown.cpp.o" \
"CMakeFiles/Opto.dir/type.cpp.o" \
"CMakeFiles/Opto.dir/editable_text.cpp.o"

# External object files for target Opto
Opto_EXTERNAL_OBJECTS =

Opto: CMakeFiles/Opto.dir/main.cpp.o
Opto: CMakeFiles/Opto.dir/object_view.cpp.o
Opto: CMakeFiles/Opto.dir/user_input.cpp.o
Opto: CMakeFiles/Opto.dir/object.cpp.o
Opto: CMakeFiles/Opto.dir/do_then.cpp.o
Opto: CMakeFiles/Opto.dir/event.cpp.o
Opto: CMakeFiles/Opto.dir/unknown.cpp.o
Opto: CMakeFiles/Opto.dir/type.cpp.o
Opto: CMakeFiles/Opto.dir/editable_text.cpp.o
Opto: CMakeFiles/Opto.dir/build.make
Opto: /opt/homebrew/Cellar/raylib/5.0/lib/libraylib.dylib
Opto: CMakeFiles/Opto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable Opto"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Opto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Opto.dir/build: Opto
.PHONY : CMakeFiles/Opto.dir/build

CMakeFiles/Opto.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Opto.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Opto.dir/clean

CMakeFiles/Opto.dir/depend:
	cd /Users/zakgriffin/CLionProjects/Opto/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zakgriffin/CLionProjects/Opto /Users/zakgriffin/CLionProjects/Opto /Users/zakgriffin/CLionProjects/Opto/cmake-build-debug /Users/zakgriffin/CLionProjects/Opto/cmake-build-debug /Users/zakgriffin/CLionProjects/Opto/cmake-build-debug/CMakeFiles/Opto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Opto.dir/depend

