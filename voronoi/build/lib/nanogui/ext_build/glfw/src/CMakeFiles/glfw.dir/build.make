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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xiezili/Desktop/voronoi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiezili/Desktop/voronoi/build

# Include any dependencies generated for this target.
include lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/depend.make

# Include the progress variables for this target.
include lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/progress.make

# Include the compile flags for this target's objects.
include lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/flags.make

# Object files for target glfw
glfw_OBJECTS =

# External object files for target glfw
glfw_EXTERNAL_OBJECTS = \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/context.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/init.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/input.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/monitor.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/vulkan.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/window.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_init.m.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_joystick.m.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_monitor.m.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_window.m.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_time.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/posix_tls.c.o" \
"/Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/nsgl_context.m.o"

lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/context.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/init.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/input.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/monitor.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/vulkan.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/window.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_init.m.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_joystick.m.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_monitor.m.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_window.m.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/cocoa_time.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/posix_tls.c.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw_objects.dir/nsgl_context.m.o
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/build.make
lib/nanogui/ext_build/glfw/src/libglfw.dylib: lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xiezili/Desktop/voronoi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking C shared library libglfw.dylib"
	cd /Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glfw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/build: lib/nanogui/ext_build/glfw/src/libglfw.dylib

.PHONY : lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/build

lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/clean:
	cd /Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src && $(CMAKE_COMMAND) -P CMakeFiles/glfw.dir/cmake_clean.cmake
.PHONY : lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/clean

lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/depend:
	cd /Users/xiezili/Desktop/voronoi/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiezili/Desktop/voronoi /Users/xiezili/Desktop/voronoi/lib/nanogui/ext/glfw/src /Users/xiezili/Desktop/voronoi/build /Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src /Users/xiezili/Desktop/voronoi/build/lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/nanogui/ext_build/glfw/src/CMakeFiles/glfw.dir/depend

