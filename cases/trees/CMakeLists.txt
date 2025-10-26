cmake_minimum_required(VERSION 3.10)

# project declaration
project(CASE_STUDY_TREES DESCRIPTION "k-dimensional data structure for fast computation of class of trees up to graph isomorphism")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# Custom functions

function(configure_tree_executable executable)

	configure_executable(${executable})

	# **************************************************************************
	# Include paths

	target_include_directories(${executable} PRIVATE ../..)
	target_include_directories(${executable} PRIVATE ${LAL_DIR})

	# **************************************************************************
	# Dependencies

	target_link_libraries(${executable} gmp)
	target_link_libraries(${executable} pthread)
	target_link_libraries(${executable} ${OpenMP_CXX_LIBRARIES})

	# Linear Arrangement Library
	target_link_libraries(${executable} ${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)

endfunction(configure_tree_executable)

# Looking for the Linear Arrangement Library
if (NOT DEFINED LAL_DIR)
	message(STATUS "LAL's development root directory was not given.")
	message(STATUS "    Please, rerun 'cmake' the location of LAL's development root directory:")
	message(STATUS "        -DLAL_DIR=/path/to/the/library")
	message(STATUS "    Make sure it is an absolute path!")
	message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
else()
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		message(STATUS "Link against laldebug (debug compilation of lal)")
		set(LAL_LIB_DIR ${LAL_DIR}/lal-debug)
		set(LAL_LIB_NAME lal)
	else()
		message(STATUS "Link against lal (release compilation of lal)")
		set(LAL_LIB_DIR ${LAL_DIR}/lal-release)
		set(LAL_LIB_NAME laloptimized)
	endif()
	message(STATUS "    LAL_LIB_DIR:  ${LAL_LIB_DIR}")
	message(STATUS "    LAL_LIB_NAME: ${LAL_LIB_NAME}")
endif()

# ******************************************************************************
# LIBRARIES
# check if libraries are installed in the system

# GMP
find_library(GMP_LIB gmp)
if (GMP_LIB)
	message(STATUS "Found gmp: ${GMP_LIB}")
else()
	message(FATAL_ERROR "Error: gmp library not found")
endif()

# pthread
find_library(PTHREAD_LIB pthread)
if (PTHREAD_LIB)
	message(STATUS "Found pthread: ${PTHREAD_LIB}")
else()
	message(FATAL_ERROR "Error: pthread library not found")
endif()

# OpenMP
find_package(OpenMP REQUIRED)

# ******************************************************************************
# MAKE EXECUTABLES

file(GLOB_RECURSE ctree ../../ctree/*.hpp)

add_executable(trees_profiling trees_profiling.cpp ${ctree})
configure_tree_executable(trees_profiling)

add_executable(trees_occupancy trees_occupancy.cpp ${ctree})
configure_tree_executable(trees_occupancy)

add_executable(trees_distribution trees_distribution.cpp ${ctree})
configure_tree_executable(trees_distribution)
