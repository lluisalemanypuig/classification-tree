cmake_minimum_required(VERSION 3.10)

# project declaration
project(EXAMPLES DESCRIPTION "Examples for ir-tree")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# Custom functions
function(add_compiler_flag executable flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		target_compile_options(${executable} PRIVATE ${flag})
	endif()
endfunction(add_compiler_flag)

function(configure_executable executable)
	# define executable's properties
	set_target_properties(${executable} PROPERTIES SOVERSION 1)
	set_target_properties(${executable} PROPERTIES LINKER_LANGUAGE CXX)

	# ******************************************************************************
	# Definitions

	add_compiler_flag(${executable} -DIR_DEBUG)

	# ******************************************************************************
	# Warning flags
	message(STATUS "Adding warning flags")
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

	# OpenMP
	add_compiler_flag(${executable} ${OpenMP_CXX_FLAGS})

	# ******************************************************************************
	# Optimisation flags
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		add_compiler_flag(${executable} -Og)
		add_compiler_flag(${executable} -DDEBUG)
		add_compiler_flag(${executable} -D_GLIBCXX_DEBUG)
	else()
		# flags added by default:
		#   -O3
		#   -DNDEBUG
		add_compiler_flag(${executable} -UDEBUG)
	endif()

	# ******************************************************************************
	# Include paths
	target_include_directories(${executable} PRIVATE ..)

	# ******************************************************************************
	# Dependencies

	# GMP
	target_link_libraries(${executable} gmp)

	# pthread
	target_link_libraries(${executable} pthread)

	# OpenMP
	target_link_libraries(${executable} ${OpenMP_CXX_LIBRARIES})
	
endfunction(configure_executable)

# ******************************************************************************
# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ******************************************************************************
# LIBRARIES
# check if libraries are installed in the system

# ***
# GMP
find_library(GMP_LIB gmp)
if (GMP_LIB)
	message(STATUS "Found gmp: ${GMP_LIB}")
else()
	message(FATAL_ERROR "Error: gmp library not found")
endif()

# *******
# pthread
find_library(PTHREAD_LIB pthread)
if (PTHREAD_LIB)
	message(STATUS "Found pthread: ${PTHREAD_LIB}")
else()
	message(FATAL_ERROR "Error: pthread library not found")
endif()

# ******
# OpenMP
find_package(OpenMP REQUIRED)

# ******************************************************************************
# MAKE EXECUTABLES

file(GLOB_RECURSE isorepr ../isorepr/*.hpp)

add_executable(example example.cpp ${isorepr})
configure_executable(example)
