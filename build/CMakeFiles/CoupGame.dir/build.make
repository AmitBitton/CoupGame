# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/amit/CPP_EX3_25-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amit/CPP_EX3_25-main/build

# Include any dependencies generated for this target.
include CMakeFiles/CoupGame.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CoupGame.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CoupGame.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CoupGame.dir/flags.make

CMakeFiles/CoupGame.dir/main.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/main.cpp.o: /home/amit/CPP_EX3_25-main/main.cpp
CMakeFiles/CoupGame.dir/main.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CoupGame.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/main.cpp.o -MF CMakeFiles/CoupGame.dir/main.cpp.o.d -o CMakeFiles/CoupGame.dir/main.cpp.o -c /home/amit/CPP_EX3_25-main/main.cpp

CMakeFiles/CoupGame.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/main.cpp > CMakeFiles/CoupGame.dir/main.cpp.i

CMakeFiles/CoupGame.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/main.cpp -o CMakeFiles/CoupGame.dir/main.cpp.s

CMakeFiles/CoupGame.dir/Game.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Game.cpp.o: /home/amit/CPP_EX3_25-main/Game.cpp
CMakeFiles/CoupGame.dir/Game.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CoupGame.dir/Game.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Game.cpp.o -MF CMakeFiles/CoupGame.dir/Game.cpp.o.d -o CMakeFiles/CoupGame.dir/Game.cpp.o -c /home/amit/CPP_EX3_25-main/Game.cpp

CMakeFiles/CoupGame.dir/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Game.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Game.cpp > CMakeFiles/CoupGame.dir/Game.cpp.i

CMakeFiles/CoupGame.dir/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Game.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Game.cpp -o CMakeFiles/CoupGame.dir/Game.cpp.s

CMakeFiles/CoupGame.dir/Player.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Player.cpp.o: /home/amit/CPP_EX3_25-main/Player.cpp
CMakeFiles/CoupGame.dir/Player.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/CoupGame.dir/Player.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Player.cpp.o -MF CMakeFiles/CoupGame.dir/Player.cpp.o.d -o CMakeFiles/CoupGame.dir/Player.cpp.o -c /home/amit/CPP_EX3_25-main/Player.cpp

CMakeFiles/CoupGame.dir/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Player.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Player.cpp > CMakeFiles/CoupGame.dir/Player.cpp.i

CMakeFiles/CoupGame.dir/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Player.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Player.cpp -o CMakeFiles/CoupGame.dir/Player.cpp.s

CMakeFiles/CoupGame.dir/Baron.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Baron.cpp.o: /home/amit/CPP_EX3_25-main/Baron.cpp
CMakeFiles/CoupGame.dir/Baron.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/CoupGame.dir/Baron.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Baron.cpp.o -MF CMakeFiles/CoupGame.dir/Baron.cpp.o.d -o CMakeFiles/CoupGame.dir/Baron.cpp.o -c /home/amit/CPP_EX3_25-main/Baron.cpp

CMakeFiles/CoupGame.dir/Baron.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Baron.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Baron.cpp > CMakeFiles/CoupGame.dir/Baron.cpp.i

CMakeFiles/CoupGame.dir/Baron.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Baron.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Baron.cpp -o CMakeFiles/CoupGame.dir/Baron.cpp.s

CMakeFiles/CoupGame.dir/General.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/General.cpp.o: /home/amit/CPP_EX3_25-main/General.cpp
CMakeFiles/CoupGame.dir/General.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/CoupGame.dir/General.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/General.cpp.o -MF CMakeFiles/CoupGame.dir/General.cpp.o.d -o CMakeFiles/CoupGame.dir/General.cpp.o -c /home/amit/CPP_EX3_25-main/General.cpp

CMakeFiles/CoupGame.dir/General.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/General.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/General.cpp > CMakeFiles/CoupGame.dir/General.cpp.i

CMakeFiles/CoupGame.dir/General.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/General.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/General.cpp -o CMakeFiles/CoupGame.dir/General.cpp.s

CMakeFiles/CoupGame.dir/Governor.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Governor.cpp.o: /home/amit/CPP_EX3_25-main/Governor.cpp
CMakeFiles/CoupGame.dir/Governor.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/CoupGame.dir/Governor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Governor.cpp.o -MF CMakeFiles/CoupGame.dir/Governor.cpp.o.d -o CMakeFiles/CoupGame.dir/Governor.cpp.o -c /home/amit/CPP_EX3_25-main/Governor.cpp

CMakeFiles/CoupGame.dir/Governor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Governor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Governor.cpp > CMakeFiles/CoupGame.dir/Governor.cpp.i

CMakeFiles/CoupGame.dir/Governor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Governor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Governor.cpp -o CMakeFiles/CoupGame.dir/Governor.cpp.s

CMakeFiles/CoupGame.dir/Judge.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Judge.cpp.o: /home/amit/CPP_EX3_25-main/Judge.cpp
CMakeFiles/CoupGame.dir/Judge.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/CoupGame.dir/Judge.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Judge.cpp.o -MF CMakeFiles/CoupGame.dir/Judge.cpp.o.d -o CMakeFiles/CoupGame.dir/Judge.cpp.o -c /home/amit/CPP_EX3_25-main/Judge.cpp

CMakeFiles/CoupGame.dir/Judge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Judge.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Judge.cpp > CMakeFiles/CoupGame.dir/Judge.cpp.i

CMakeFiles/CoupGame.dir/Judge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Judge.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Judge.cpp -o CMakeFiles/CoupGame.dir/Judge.cpp.s

CMakeFiles/CoupGame.dir/Merchant.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Merchant.cpp.o: /home/amit/CPP_EX3_25-main/Merchant.cpp
CMakeFiles/CoupGame.dir/Merchant.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/CoupGame.dir/Merchant.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Merchant.cpp.o -MF CMakeFiles/CoupGame.dir/Merchant.cpp.o.d -o CMakeFiles/CoupGame.dir/Merchant.cpp.o -c /home/amit/CPP_EX3_25-main/Merchant.cpp

CMakeFiles/CoupGame.dir/Merchant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Merchant.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Merchant.cpp > CMakeFiles/CoupGame.dir/Merchant.cpp.i

CMakeFiles/CoupGame.dir/Merchant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Merchant.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Merchant.cpp -o CMakeFiles/CoupGame.dir/Merchant.cpp.s

CMakeFiles/CoupGame.dir/Spy.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/Spy.cpp.o: /home/amit/CPP_EX3_25-main/Spy.cpp
CMakeFiles/CoupGame.dir/Spy.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/CoupGame.dir/Spy.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/Spy.cpp.o -MF CMakeFiles/CoupGame.dir/Spy.cpp.o.d -o CMakeFiles/CoupGame.dir/Spy.cpp.o -c /home/amit/CPP_EX3_25-main/Spy.cpp

CMakeFiles/CoupGame.dir/Spy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/Spy.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/Spy.cpp > CMakeFiles/CoupGame.dir/Spy.cpp.i

CMakeFiles/CoupGame.dir/Spy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/Spy.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/Spy.cpp -o CMakeFiles/CoupGame.dir/Spy.cpp.s

CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o: /home/amit/CPP_EX3_25-main/GUI/GameWindow.cpp
CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o -MF CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o.d -o CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o -c /home/amit/CPP_EX3_25-main/GUI/GameWindow.cpp

CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/GUI/GameWindow.cpp > CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.i

CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/GUI/GameWindow.cpp -o CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.s

CMakeFiles/CoupGame.dir/GUI/Button.cpp.o: CMakeFiles/CoupGame.dir/flags.make
CMakeFiles/CoupGame.dir/GUI/Button.cpp.o: /home/amit/CPP_EX3_25-main/GUI/Button.cpp
CMakeFiles/CoupGame.dir/GUI/Button.cpp.o: CMakeFiles/CoupGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/CoupGame.dir/GUI/Button.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CoupGame.dir/GUI/Button.cpp.o -MF CMakeFiles/CoupGame.dir/GUI/Button.cpp.o.d -o CMakeFiles/CoupGame.dir/GUI/Button.cpp.o -c /home/amit/CPP_EX3_25-main/GUI/Button.cpp

CMakeFiles/CoupGame.dir/GUI/Button.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CoupGame.dir/GUI/Button.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/CPP_EX3_25-main/GUI/Button.cpp > CMakeFiles/CoupGame.dir/GUI/Button.cpp.i

CMakeFiles/CoupGame.dir/GUI/Button.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CoupGame.dir/GUI/Button.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/CPP_EX3_25-main/GUI/Button.cpp -o CMakeFiles/CoupGame.dir/GUI/Button.cpp.s

# Object files for target CoupGame
CoupGame_OBJECTS = \
"CMakeFiles/CoupGame.dir/main.cpp.o" \
"CMakeFiles/CoupGame.dir/Game.cpp.o" \
"CMakeFiles/CoupGame.dir/Player.cpp.o" \
"CMakeFiles/CoupGame.dir/Baron.cpp.o" \
"CMakeFiles/CoupGame.dir/General.cpp.o" \
"CMakeFiles/CoupGame.dir/Governor.cpp.o" \
"CMakeFiles/CoupGame.dir/Judge.cpp.o" \
"CMakeFiles/CoupGame.dir/Merchant.cpp.o" \
"CMakeFiles/CoupGame.dir/Spy.cpp.o" \
"CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o" \
"CMakeFiles/CoupGame.dir/GUI/Button.cpp.o"

# External object files for target CoupGame
CoupGame_EXTERNAL_OBJECTS =

CoupGame: CMakeFiles/CoupGame.dir/main.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Game.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Player.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Baron.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/General.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Governor.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Judge.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Merchant.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/Spy.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/GUI/GameWindow.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/GUI/Button.cpp.o
CoupGame: CMakeFiles/CoupGame.dir/build.make
CoupGame: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.6.1
CoupGame: /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.6.1
CoupGame: /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.6.1
CoupGame: CMakeFiles/CoupGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/amit/CPP_EX3_25-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable CoupGame"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CoupGame.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CoupGame.dir/build: CoupGame
.PHONY : CMakeFiles/CoupGame.dir/build

CMakeFiles/CoupGame.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CoupGame.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CoupGame.dir/clean

CMakeFiles/CoupGame.dir/depend:
	cd /home/amit/CPP_EX3_25-main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amit/CPP_EX3_25-main /home/amit/CPP_EX3_25-main /home/amit/CPP_EX3_25-main/build /home/amit/CPP_EX3_25-main/build /home/amit/CPP_EX3_25-main/build/CMakeFiles/CoupGame.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/CoupGame.dir/depend

