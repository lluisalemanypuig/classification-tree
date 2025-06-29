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

add_executable(depth_0__empty depth_0__empty.cpp test_definitions.hpp ${ctree})
configure_executable(depth_0__empty)
add_test(NAME depth_0__empty COMMAND depth_0__empty)

add_executable(depth_0__eq__all depth_0__eq__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_0__eq__all)
add_test(NAME depth_0__eq__all COMMAND depth_0__eq__all)

add_executable(depth_0__eq__unique depth_0__eq__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_0__eq__unique)
add_test(NAME depth_0__eq__unique COMMAND depth_0__eq__unique)

add_executable(depth_0__lt__all depth_0__lt__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_0__lt__all)
add_test(NAME depth_0__lt__all COMMAND depth_0__lt__all)

add_executable(depth_0__lt__unique depth_0__lt__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_0__lt__unique)
add_test(NAME depth_0__lt__unique COMMAND depth_0__lt__unique)

# DEPTH 1

add_executable(depth_1__empty depth_1__empty.cpp test_definitions.hpp ${ctree})
configure_executable(depth_1__empty)
add_test(NAME depth_1__empty COMMAND depth_1__empty)

add_executable(depth_1__eq__all depth_1__eq__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_1__eq__all)
add_test(NAME depth_1__eq__all COMMAND depth_1__eq__all)

add_executable(depth_1__eq__unique depth_1__eq__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_1__eq__unique)
add_test(NAME depth_1__eq__unique COMMAND depth_1__eq__unique)

add_executable(depth_1__lt__all depth_1__lt__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_1__lt__all)
add_test(NAME depth_1__lt__all COMMAND depth_1__lt__all)

add_executable(depth_1__lt__unique depth_1__lt__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_1__lt__unique)
add_test(NAME depth_1__lt__unique COMMAND depth_1__lt__unique)

# DEPTH 3

add_executable(depth_3__empty depth_3__empty.cpp test_definitions.hpp ${ctree})
configure_executable(depth_3__empty)
add_test(NAME depth_3__empty COMMAND depth_3__empty)

add_executable(depth_3__eq__all depth_3__eq__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_3__eq__all)
add_test(NAME depth_3__eq__all COMMAND depth_3__eq__all)

add_executable(depth_3__eq__unique depth_3__eq__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_3__eq__unique)
add_test(NAME depth_3__eq__unique COMMAND depth_3__eq__unique)

add_executable(depth_3__lt__all depth_3__lt__all.cpp test_definitions.hpp ${ctree})
configure_executable(depth_3__lt__all)
add_test(NAME depth_3__lt__all COMMAND depth_3__lt__all)

add_executable(depth_3__lt__unique depth_3__lt__unique.cpp test_definitions.hpp ${ctree})
configure_executable(depth_3__lt__unique)
add_test(NAME depth_3__lt__unique COMMAND depth_3__lt__unique)
