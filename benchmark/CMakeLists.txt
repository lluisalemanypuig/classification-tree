cmake_minimum_required(VERSION 3.16)

message(STATUS "===============================")
message(STATUS "Configuring benchmark for CTREE")
message(STATUS "===============================")

# project declaration
project(CTREETESTS VERSION "${CTREE_MAJOR}.${CTREE_PATCH}")
project(CTREETESTS DESCRIPTION "Benchmakrs for classification tree")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories(platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# Custom functions

function(configure_benchmark_executable executable)

	configure_executable(${executable})

	# libraries
	target_link_libraries(${executable} benchmark::benchmark)
	target_link_libraries(${executable}	pthread)

endfunction(configure_benchmark_executable)

# ******************************************************************************
# Set default build type

set(default_build_type "Release")

# ******************************************************************************
# LIBRARIES
# check if libraries are installed in the system

# benchmark (https://github.com/google/benchmark)
find_package(benchmark REQUIRED)

# pthread
find_library(pthread_LIB pthread)
if (pthread_LIB)
	message(STATUS "Found pthread: ${pthread_LIB}")
else()
	message(FATAL_ERROR "Error: pthread library not found")
endif()

#******************************************************************************
# MAKE EXECUTABLES

file(GLOB_RECURSE ctree ../ctree/*.hpp)

add_executable(search search.cpp ${ctree})
configure_benchmark_executable(search)
