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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alina/duende

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alina/duende/cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/test_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_server.dir/flags.make

CMakeFiles/test_server.dir/profile_server.cpp.o: CMakeFiles/test_server.dir/flags.make
CMakeFiles/test_server.dir/profile_server.cpp.o: /Users/alina/duende/profile_server.cpp
CMakeFiles/test_server.dir/profile_server.cpp.o: CMakeFiles/test_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/alina/duende/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_server.dir/profile_server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_server.dir/profile_server.cpp.o -MF CMakeFiles/test_server.dir/profile_server.cpp.o.d -o CMakeFiles/test_server.dir/profile_server.cpp.o -c /Users/alina/duende/profile_server.cpp

CMakeFiles/test_server.dir/profile_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_server.dir/profile_server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alina/duende/profile_server.cpp > CMakeFiles/test_server.dir/profile_server.cpp.i

CMakeFiles/test_server.dir/profile_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_server.dir/profile_server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alina/duende/profile_server.cpp -o CMakeFiles/test_server.dir/profile_server.cpp.s

CMakeFiles/test_server.dir/profile.grpc.pb.cc.o: CMakeFiles/test_server.dir/flags.make
CMakeFiles/test_server.dir/profile.grpc.pb.cc.o: profile.grpc.pb.cc
CMakeFiles/test_server.dir/profile.grpc.pb.cc.o: CMakeFiles/test_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/alina/duende/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_server.dir/profile.grpc.pb.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_server.dir/profile.grpc.pb.cc.o -MF CMakeFiles/test_server.dir/profile.grpc.pb.cc.o.d -o CMakeFiles/test_server.dir/profile.grpc.pb.cc.o -c /Users/alina/duende/cmake/build/profile.grpc.pb.cc

CMakeFiles/test_server.dir/profile.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_server.dir/profile.grpc.pb.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alina/duende/cmake/build/profile.grpc.pb.cc > CMakeFiles/test_server.dir/profile.grpc.pb.cc.i

CMakeFiles/test_server.dir/profile.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_server.dir/profile.grpc.pb.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alina/duende/cmake/build/profile.grpc.pb.cc -o CMakeFiles/test_server.dir/profile.grpc.pb.cc.s

CMakeFiles/test_server.dir/profile.pb.cc.o: CMakeFiles/test_server.dir/flags.make
CMakeFiles/test_server.dir/profile.pb.cc.o: profile.pb.cc
CMakeFiles/test_server.dir/profile.pb.cc.o: CMakeFiles/test_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/alina/duende/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_server.dir/profile.pb.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_server.dir/profile.pb.cc.o -MF CMakeFiles/test_server.dir/profile.pb.cc.o.d -o CMakeFiles/test_server.dir/profile.pb.cc.o -c /Users/alina/duende/cmake/build/profile.pb.cc

CMakeFiles/test_server.dir/profile.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_server.dir/profile.pb.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alina/duende/cmake/build/profile.pb.cc > CMakeFiles/test_server.dir/profile.pb.cc.i

CMakeFiles/test_server.dir/profile.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_server.dir/profile.pb.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alina/duende/cmake/build/profile.pb.cc -o CMakeFiles/test_server.dir/profile.pb.cc.s

# Object files for target test_server
test_server_OBJECTS = \
"CMakeFiles/test_server.dir/profile_server.cpp.o" \
"CMakeFiles/test_server.dir/profile.grpc.pb.cc.o" \
"CMakeFiles/test_server.dir/profile.pb.cc.o"

# External object files for target test_server
test_server_EXTERNAL_OBJECTS =

test_server: CMakeFiles/test_server.dir/profile_server.cpp.o
test_server: CMakeFiles/test_server.dir/profile.grpc.pb.cc.o
test_server: CMakeFiles/test_server.dir/profile.pb.cc.o
test_server: CMakeFiles/test_server.dir/build.make
test_server: /opt/homebrew/Cellar/openssl@3/3.2.1/lib/libcrypto.dylib
test_server: /opt/homebrew/lib/libgrpc++.1.60.0.dylib
test_server: /opt/homebrew/lib/libprotobuf.25.3.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_check_op.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_leak_check.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_die_if_null.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_conditions.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_message.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_nullguard.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_examine_stack.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_format.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_proto.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_log_sink_set.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_sink.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_entry.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_initialize.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_globals.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_internal_globals.2308.0.0.dylib
test_server: /opt/homebrew/lib/libgrpc.37.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_statusor.2308.0.0.dylib
test_server: /opt/homebrew/lib/libupb_collections_lib.37.0.0.dylib
test_server: /opt/homebrew/lib/libupb_json_lib.37.0.0.dylib
test_server: /opt/homebrew/lib/libupb_textformat_lib.37.0.0.dylib
test_server: /opt/homebrew/lib/libutf8_range_lib.37.0.0.dylib
test_server: /opt/homebrew/lib/libupb.37.0.0.dylib
test_server: /opt/homebrew/lib/libre2.11.0.0.dylib
test_server: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.0.sdk/usr/lib/libz.tbd
test_server: /opt/homebrew/lib/libcares.2.11.0.dylib
test_server: /opt/homebrew/lib/libgpr.37.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_reflection.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_raw_hash_set.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_hash.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_city.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_low_level_hash.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_hashtablez_sampler.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_config.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_program_name.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_private_handle_accessor.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_commandlineflag.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_commandlineflag_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_status.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_cord.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_cordz_info.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_cord_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_cordz_functions.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_exponential_biased.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_cordz_handle.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_crc_cord_state.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_crc32c.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_crc_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_crc_cpu_detect.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_strerror.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_synchronization.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_stacktrace.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_symbolize.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_debugging_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_demangle_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_graphcycles_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_kernel_timeout_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_malloc_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_time.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_civil_time.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_time_zone.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_bad_variant_access.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_flags_marshalling.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_str_format_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_distributions.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_seed_sequences.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_pool_urbg.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_randen.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_randen_hwaes.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_randen_hwaes_impl.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_randen_slow.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_platform.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_internal_seed_material.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_bad_optional_access.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_strings.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_int128.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_string_view.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_throw_delegate.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_strings_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_base.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_spinlock_wait.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_raw_logging_internal.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_log_severity.2308.0.0.dylib
test_server: /opt/homebrew/lib/libabsl_random_seed_gen_exception.2308.0.0.dylib
test_server: /opt/homebrew/Cellar/openssl@3/3.2.1/lib/libssl.dylib
test_server: /opt/homebrew/Cellar/openssl@3/3.2.1/lib/libcrypto.dylib
test_server: /opt/homebrew/lib/libaddress_sorting.37.0.0.dylib
test_server: CMakeFiles/test_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/alina/duende/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable test_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_server.dir/build: test_server
.PHONY : CMakeFiles/test_server.dir/build

CMakeFiles/test_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_server.dir/clean

CMakeFiles/test_server.dir/depend:
	cd /Users/alina/duende/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alina/duende /Users/alina/duende /Users/alina/duende/cmake/build /Users/alina/duende/cmake/build /Users/alina/duende/cmake/build/CMakeFiles/test_server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_server.dir/depend

