cmake_minimum_required(VERSION 3.16)
project(ClassificationTree LANGUAGES CXX)

# Turn on export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project's version
set(CTREE_MAJOR 99.99)
set(CTREE_PATCH 99)

# ******************************************************************************
# Custom functions

function(add_compiler_flag executable flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	if (CXX_HAS_FLAG_${flag})
		target_compile_options(${executable} PRIVATE ${flag})
	endif()
endfunction(add_compiler_flag)

function(define_symbol executable symbol)
	target_compile_options(${executable} PRIVATE ${symbol})
endfunction(define_symbol)

function(configure_executable executable)
	# define executable's properties
	set_target_properties(${executable} PROPERTIES SOVERSION 1)
	set_target_properties(${executable} PROPERTIES LINKER_LANGUAGE CXX)

	# ******************************************************************************
	# Definitions

	# ******************************************************************************
	# Warning flags

	add_compiler_flag(${executable} -Wall)
	add_compiler_flag(${executable} -Wextra)
	add_compiler_flag(${executable} -Wshadow)
	add_compiler_flag(${executable} -Wnon-virtual-dtor)
	add_compiler_flag(${executable} -Wold-style-cast)
	add_compiler_flag(${executable} -Wcast-align)
	add_compiler_flag(${executable} -Wunused)
	add_compiler_flag(${executable} -Woverloaded-virtual)
	add_compiler_flag(${executable} -Wpedantic)
	add_compiler_flag(${executable} -Wconversion)
	add_compiler_flag(${executable} -Wsign-conversion)
	add_compiler_flag(${executable} -Wnull-dereference)
	add_compiler_flag(${executable} -Wdouble-promotion)
	add_compiler_flag(${executable} -Wformat=2)
	add_compiler_flag(${executable} -Wduplicated-cond)
	add_compiler_flag(${executable} -Wduplicated-branches)
	add_compiler_flag(${executable} -Wlogical-op)
	add_compiler_flag(${executable} -Wuseless-cast)
	add_compiler_flag(${executable} -Wrestrict)

	# ******************************************************************************
	# Optimisation flags

	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		define_symbol(${executable} -Og)
		define_symbol(${executable} -DDEBUG)
		define_symbol(${executable} -D_GLIBCXX_DEBUG)
	else()
		# flags added by default:
		#   -O3
		#   -DNDEBUG
		define_symbol(${executable} -UDEBUG)
	endif()

	# ***************************************************************************
	# Include paths

	target_include_directories(${executable} PRIVATE ${CTREE_DIR})

endfunction(configure_executable)

# ******************************************************************************
# Set default build type

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(
		CMAKE_BUILD_TYPE "${default_build_type}"
		CACHE STRING "Choose the type of build."
		FORCE
	)
	# Set the possible values of build type for cmake-gui
	set_property(
		CACHE CMAKE_BUILD_TYPE
		PROPERTY
			STRINGS "Debug" "Release"
	)
endif()

# ******************************************************************************
# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ******************************************************************************
# Path to the current CMakeFile

set(CTREE_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "CTREE_DIR: ${CTREE_DIR}")

# ******************************************************************************

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Link against ctree (debug compilation of ctree)")
	set(CTREE_LIB_DIR ${CTREE_DIR}/build-debug/ctree)
	set(CTREE_LIB_NAME ctree)
else()
	message(STATUS "Link against ctreeotimized (release compilation of ctree)")
	set(CTREE_LIB_DIR ${CTREE_DIR}/build-release/ctree)
	set(CTREE_LIB_NAME ctreeoptimized)
endif()
message(STATUS "    CTREE_LIB_DIR:  ${CTREE_LIB_DIR}")
message(STATUS "    CTREE_LIB_NAME: ${CTREE_LIB_NAME}")

# *******************************************************************************
# Add subdirectories

add_subdirectory(tests)
add_subdirectory(cases)

# *******************************************************************************
# Other

set(COMMANDS_FILE_ORIG ${CMAKE_BUILD_TYPE}/compile_commands.json)
set(COMMANDS_FILE_DEST compile_commands.json)
if(EXISTS "${COMMANDS_FILE_ORIG}")
	file(COPY_FILE "${COMMANDS_FILE_ORIG}" "${COMMANDS_FILE_DEST}")
endif()
