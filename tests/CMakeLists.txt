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

# DEPTH 0

add_executable(test_depth_0__empty test_depth_0__empty.cpp definitions.hpp ${ctree})
configure_executable(test_depth_0__empty)
add_test(NAME test_depth_0__empty COMMAND test_depth_0__empty)

add_executable(test_depth_0__eq__all test_depth_0__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_0__eq__all)
add_test(NAME test_depth_0__eq__all COMMAND test_depth_0__eq__all)

add_executable(test_depth_0__eq__unique test_depth_0__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_0__eq__unique)
add_test(NAME test_depth_0__eq__unique COMMAND test_depth_0__eq__unique)

add_executable(test_depth_0__lt__all test_depth_0__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_0__lt__all)
add_test(NAME test_depth_0__lt__all COMMAND test_depth_0__lt__all)

add_executable(test_depth_0__lt__unique test_depth_0__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_0__lt__unique)
add_test(NAME test_depth_0__lt__unique COMMAND test_depth_0__lt__unique)

# DEPTH 1

add_executable(test_depth_1__empty test_depth_1__empty.cpp definitions.hpp ${ctree})
configure_executable(test_depth_1__empty)
add_test(NAME test_depth_1__empty COMMAND test_depth_1__empty)

add_executable(test_depth_1__eq__all test_depth_1__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_1__eq__all)
add_test(NAME test_depth_1__eq__all COMMAND test_depth_1__eq__all)

add_executable(test_depth_1__eq__unique test_depth_1__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_1__eq__unique)
add_test(NAME test_depth_1__eq__unique COMMAND test_depth_1__eq__unique)

add_executable(test_depth_1__lt__all test_depth_1__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_1__lt__all)
add_test(NAME test_depth_1__lt__all COMMAND test_depth_1__lt__all)

add_executable(test_depth_1__lt__unique test_depth_1__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_1__lt__unique)
add_test(NAME test_depth_1__lt__unique COMMAND test_depth_1__lt__unique)

# DEPTH 3

add_executable(test_depth_3__empty test_depth_3__empty.cpp definitions.hpp ${ctree})
configure_executable(test_depth_3__empty)
add_test(NAME test_depth_3__empty COMMAND test_depth_3__empty)

add_executable(test_depth_3__eq__all test_depth_3__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_3__eq__all)
add_test(NAME test_depth_3__eq__all COMMAND test_depth_3__eq__all)

add_executable(test_depth_3__eq__unique test_depth_3__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_3__eq__unique)
add_test(NAME test_depth_3__eq__unique COMMAND test_depth_3__eq__unique)

add_executable(test_depth_3__lt__all test_depth_3__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test_depth_3__lt__all)
add_test(NAME test_depth_3__lt__all COMMAND test_depth_3__lt__all)

add_executable(test_depth_3__lt__unique test_depth_3__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test_depth_3__lt__unique)
add_test(NAME test_depth_3__lt__unique COMMAND test_depth_3__lt__unique)
