# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /make/rabbitmq-c-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /make/rabbitmq-c-master/temp

# Include any dependencies generated for this target.
include examples/CMakeFiles/amqps_exchange_declare.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/amqps_exchange_declare.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/amqps_exchange_declare.dir/flags.make

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o: examples/CMakeFiles/amqps_exchange_declare.dir/flags.make
examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o: ../examples/amqps_exchange_declare.c
	$(CMAKE_COMMAND) -E cmake_progress_report /make/rabbitmq-c-master/temp/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o   -c /make/rabbitmq-c-master/examples/amqps_exchange_declare.c

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.i"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /make/rabbitmq-c-master/examples/amqps_exchange_declare.c > CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.i

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.s"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /make/rabbitmq-c-master/examples/amqps_exchange_declare.c -o CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.s

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.requires:
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.requires

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.provides: examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqps_exchange_declare.dir/build.make examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.provides.build
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.provides

examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.provides.build: examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o: examples/CMakeFiles/amqps_exchange_declare.dir/flags.make
examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o: ../examples/utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report /make/rabbitmq-c-master/temp/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqps_exchange_declare.dir/utils.c.o   -c /make/rabbitmq-c-master/examples/utils.c

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqps_exchange_declare.dir/utils.c.i"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /make/rabbitmq-c-master/examples/utils.c > CMakeFiles/amqps_exchange_declare.dir/utils.c.i

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqps_exchange_declare.dir/utils.c.s"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /make/rabbitmq-c-master/examples/utils.c -o CMakeFiles/amqps_exchange_declare.dir/utils.c.s

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.requires:
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.requires

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.provides: examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqps_exchange_declare.dir/build.make examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.provides.build
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.provides

examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.provides.build: examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o: examples/CMakeFiles/amqps_exchange_declare.dir/flags.make
examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o: ../examples/unix/platform_utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report /make/rabbitmq-c-master/temp/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o   -c /make/rabbitmq-c-master/examples/unix/platform_utils.c

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.i"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /make/rabbitmq-c-master/examples/unix/platform_utils.c > CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.i

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.s"
	cd /make/rabbitmq-c-master/temp/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /make/rabbitmq-c-master/examples/unix/platform_utils.c -o CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.s

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.requires:
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.requires

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.provides: examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.requires
	$(MAKE) -f examples/CMakeFiles/amqps_exchange_declare.dir/build.make examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.provides.build
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.provides

examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.provides.build: examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o

# Object files for target amqps_exchange_declare
amqps_exchange_declare_OBJECTS = \
"CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o" \
"CMakeFiles/amqps_exchange_declare.dir/utils.c.o" \
"CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o"

# External object files for target amqps_exchange_declare
amqps_exchange_declare_EXTERNAL_OBJECTS =

examples/amqps_exchange_declare: examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o
examples/amqps_exchange_declare: examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o
examples/amqps_exchange_declare: examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o
examples/amqps_exchange_declare: examples/CMakeFiles/amqps_exchange_declare.dir/build.make
examples/amqps_exchange_declare: librabbitmq/librabbitmq.so.4.1.2
examples/amqps_exchange_declare: /usr/lib64/libssl.so
examples/amqps_exchange_declare: /usr/lib64/libcrypto.so
examples/amqps_exchange_declare: examples/CMakeFiles/amqps_exchange_declare.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable amqps_exchange_declare"
	cd /make/rabbitmq-c-master/temp/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/amqps_exchange_declare.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/amqps_exchange_declare.dir/build: examples/amqps_exchange_declare
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/build

examples/CMakeFiles/amqps_exchange_declare.dir/requires: examples/CMakeFiles/amqps_exchange_declare.dir/amqps_exchange_declare.c.o.requires
examples/CMakeFiles/amqps_exchange_declare.dir/requires: examples/CMakeFiles/amqps_exchange_declare.dir/utils.c.o.requires
examples/CMakeFiles/amqps_exchange_declare.dir/requires: examples/CMakeFiles/amqps_exchange_declare.dir/unix/platform_utils.c.o.requires
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/requires

examples/CMakeFiles/amqps_exchange_declare.dir/clean:
	cd /make/rabbitmq-c-master/temp/examples && $(CMAKE_COMMAND) -P CMakeFiles/amqps_exchange_declare.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/clean

examples/CMakeFiles/amqps_exchange_declare.dir/depend:
	cd /make/rabbitmq-c-master/temp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /make/rabbitmq-c-master /make/rabbitmq-c-master/examples /make/rabbitmq-c-master/temp /make/rabbitmq-c-master/temp/examples /make/rabbitmq-c-master/temp/examples/CMakeFiles/amqps_exchange_declare.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/amqps_exchange_declare.dir/depend

