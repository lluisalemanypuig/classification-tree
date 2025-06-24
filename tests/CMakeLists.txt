cmake_minimum_required(VERSION 3.16)

message(STATUS "===========================")
message(STATUS "Configuring tests for CTREE")
message(STATUS "===========================")

# project declaration
project(CTREETESTS VERSION "${CTREE_MAJOR}.${CTREE_PATCH}")
project(CTREETESTS DESCRIPTION "Tests for classification tree")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)
# include CTest for testing
include(CTest)

# ******************************************************************************
# Custom functions

function(configure_test_executable executable)

	define_symbol(${executable} -DCTREE_DEBUG)

	# doctest
	target_link_libraries(${executable} doctest::doctest)

	# configure executable normally
	configure_executable(${executable})

endfunction(configure_test_executable)

# ******************************************************************************
# Set default build type

set(default_build_type "Release")

# ******************************************************************************
# LIBRARIES
# check if libraries are installed in the system

# *******
# doctest
find_package(doctest REQUIRED)

# ******************************************************************************
# MAKE EXECUTABLES

file(GLOB_RECURSE ctree ../ctree/*.hpp)

add_executable(test_depth_0__empty depth_0__empty.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_0__empty)
add_test(NAME test_depth_0__empty COMMAND test_depth_0__empty)

add_executable(test_depth_0__eq depth_0__eq.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_0__eq)
add_test(NAME test_depth_0__eq COMMAND test_depth_0__eq)

add_executable(test_depth_0__lt depth_0__lt.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_0__lt)
add_test(NAME test_depth_0__lt COMMAND test_depth_0__lt)

add_executable(test_depth_3__empty depth_3__empty.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_3__empty)
add_test(NAME test_depth_3__empty COMMAND test_depth_3__empty)

add_executable(test_depth_3__eq depth_3__eq.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_3__eq)
add_test(NAME test_depth_3__eq COMMAND test_depth_3__eq)

add_executable(test_depth_3__lt depth_3__lt.cpp test_definitions.hpp ${ctree})
configure_executable(test_depth_3__lt)
add_test(NAME test_depth_3__lt COMMAND test_depth_3__lt)
