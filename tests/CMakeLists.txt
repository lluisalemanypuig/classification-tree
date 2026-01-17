cmake_minimum_required(VERSION 3.16)

message(STATUS "===========================")
message(STATUS "Configuring tests for CTREE")
message(STATUS "===========================")

#project declaration
project(CTREETESTS VERSION "${CTREE_MAJOR}.${CTREE_PATCH}")
project(CTREETESTS DESCRIPTION "Tests for classification tree")

#needed includes to make CMake work
include(CheckCXXCompilerFlag)
#installation directories(platform dependent)
include(GNUInstallDirs)
#include CTest for testing
include(CTest)

#******************************************************************************
#Custom functions

function(configure_test_executable executable)

	define_symbol(${executable} -DCTREE_DEBUG)

#doctest
	target_link_libraries(${executable} doctest::doctest)

#configure executable normally
	configure_executable(${executable})

endfunction(configure_test_executable)

#******************************************************************************
#Set default build type

set(default_build_type "Release")

#******************************************************************************
#LIBRARIES
#check if libraries are installed in the system

#doctest
find_package(doctest REQUIRED)

#******************************************************************************
#MAKE EXECUTABLES

file(GLOB_RECURSE ctree ../ctree/*.hpp)

# =======
# DEPTH 0
# =======

# WITH METADATA

add_executable(test__metadata_w__depth_0__empty test__metadata_w__depth_0__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__empty)
add_test(NAME test__metadata_w__depth_0__empty COMMAND test__metadata_w__depth_0__empty)

add_executable(test__metadata_w__depth_0__eq__all test__metadata_w__depth_0__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__eq__all)
add_test(NAME test__metadata_w__depth_0__eq__all COMMAND test__metadata_w__depth_0__eq__all)

add_executable(test__metadata_w__depth_0__eq__unique test__metadata_w__depth_0__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__eq__unique)
add_test(NAME test__metadata_w__depth_0__eq__unique COMMAND test__metadata_w__depth_0__eq__unique)

add_executable(test__metadata_w__depth_0__lt__all test__metadata_w__depth_0__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__lt__all)
add_test(NAME test__metadata_w__depth_0__lt__all COMMAND test__metadata_w__depth_0__lt__all)

add_executable(test__metadata_w__depth_0__lt__unique test__metadata_w__depth_0__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__lt__unique)
add_test(NAME test__metadata_w__depth_0__lt__unique COMMAND test__metadata_w__depth_0__lt__unique)

add_executable(test__metadata_w__depth_0__merge test__metadata_w__depth_0__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_0__merge)
add_test(NAME test__metadata_w__depth_0__merge COMMAND test__metadata_w__depth_0__merge)

# WITHOUT METADATA

add_executable(test__metadata_wo__depth_0__empty test__metadata_wo__depth_0__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__empty)
add_test(NAME test__metadata_wo__depth_0__empty COMMAND test__metadata_wo__depth_0__empty)

add_executable(test__metadata_wo__depth_0__eq__all test__metadata_wo__depth_0__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__eq__all)
add_test(NAME test__metadata_wo__depth_0__eq__all COMMAND test__metadata_wo__depth_0__eq__all)

add_executable(test__metadata_wo__depth_0__eq__unique test__metadata_wo__depth_0__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__eq__unique)
add_test(NAME test__metadata_wo__depth_0__eq__unique COMMAND test__metadata_wo__depth_0__eq__unique)

add_executable(test__metadata_wo__depth_0__lt__all test__metadata_wo__depth_0__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__lt__all)
add_test(NAME test__metadata_wo__depth_0__lt__all COMMAND test__metadata_wo__depth_0__lt__all)

add_executable(test__metadata_wo__depth_0__lt__unique test__metadata_wo__depth_0__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__lt__unique)
add_test(NAME test__metadata_wo__depth_0__lt__unique COMMAND test__metadata_wo__depth_0__lt__unique)

add_executable(test__metadata_wo__depth_0__merge test__metadata_wo__depth_0__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_0__merge)
add_test(NAME test__metadata_wo__depth_0__merge COMMAND test__metadata_wo__depth_0__merge)

# =======
# DEPTH 1
# =======

# WITH METADATA

add_executable(test__metadata_w__depth_1__empty test__metadata_w__depth_1__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__empty)
add_test(NAME test__metadata_w__depth_1__empty COMMAND test__metadata_w__depth_1__empty)

add_executable(test__metadata_w__depth_1__eq__all test__metadata_w__depth_1__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__eq__all)
add_test(NAME test__metadata_w__depth_1__eq__all COMMAND test__metadata_w__depth_1__eq__all)

add_executable(test__metadata_w__depth_1__eq__unique test__metadata_w__depth_1__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__eq__unique)
add_test(NAME test__metadata_w__depth_1__eq__unique COMMAND test__metadata_w__depth_1__eq__unique)

add_executable(test__metadata_w__depth_1__lt__all test__metadata_w__depth_1__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__lt__all)
add_test(NAME test__metadata_w__depth_1__lt__all COMMAND test__metadata_w__depth_1__lt__all)

add_executable(test__metadata_w__depth_1__lt__unique test__metadata_w__depth_1__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__lt__unique)
add_test(NAME test__metadata_w__depth_1__lt__unique COMMAND test__metadata_w__depth_1__lt__unique)

add_executable(test__metadata_w__depth_1__merge test__metadata_w__depth_1__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_1__merge)
add_test(NAME test__metadata_w__depth_1__merge COMMAND test__metadata_w__depth_1__merge)

# WITHOUT METADATA

add_executable(test__metadata_wo__depth_1__empty test__metadata_wo__depth_1__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__empty)
add_test(NAME test__metadata_wo__depth_1__empty COMMAND test__metadata_wo__depth_1__empty)

add_executable(test__metadata_wo__depth_1__eq__all test__metadata_wo__depth_1__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__eq__all)
add_test(NAME test__metadata_wo__depth_1__eq__all COMMAND test__metadata_wo__depth_1__eq__all)

add_executable(test__metadata_wo__depth_1__eq__unique test__metadata_wo__depth_1__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__eq__unique)
add_test(NAME test__metadata_wo__depth_1__eq__unique COMMAND test__metadata_wo__depth_1__eq__unique)

add_executable(test__metadata_wo__depth_1__lt__all test__metadata_wo__depth_1__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__lt__all)
add_test(NAME test__metadata_wo__depth_1__lt__all COMMAND test__metadata_wo__depth_1__lt__all)

add_executable(test__metadata_wo__depth_1__lt__unique test__metadata_wo__depth_1__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__lt__unique)
add_test(NAME test__metadata_wo__depth_1__lt__unique COMMAND test__metadata_wo__depth_1__lt__unique)

add_executable(test__metadata_wo__depth_1__merge test__metadata_wo__depth_1__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_1__merge)
add_test(NAME test__metadata_wo__depth_1__merge COMMAND test__metadata_wo__depth_1__merge)

# =======
# DEPTH 3
# =======

# WITH METADATA

add_executable(test__metadata_w__depth_3__empty test__metadata_w__depth_3__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__empty)
add_test(NAME test__metadata_w__depth_3__empty COMMAND test__metadata_w__depth_3__empty)

add_executable(test__metadata_w__depth_3__eq__all test__metadata_w__depth_3__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__eq__all)
add_test(NAME test__metadata_w__depth_3__eq__all COMMAND test__metadata_w__depth_3__eq__all)

add_executable(test__metadata_w__depth_3__eq__unique test__metadata_w__depth_3__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__eq__unique)
add_test(NAME test__metadata_w__depth_3__eq__unique COMMAND test__metadata_w__depth_3__eq__unique)

add_executable(test__metadata_w__depth_3__lt__all test__metadata_w__depth_3__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__lt__all)
add_test(NAME test__metadata_w__depth_3__lt__all COMMAND test__metadata_w__depth_3__lt__all)

add_executable(test__metadata_w__depth_3__lt__unique test__metadata_w__depth_3__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__lt__unique)
add_test(NAME test__metadata_w__depth_3__lt__unique COMMAND test__metadata_w__depth_3__lt__unique)

add_executable(test__metadata_w__depth_3__merge test__metadata_w__depth_3__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_w__depth_3__merge)
add_test(NAME test__metadata_w__depth_3__merge COMMAND test__metadata_w__depth_3__merge)

# WITHOUT METADATA

add_executable(test__metadata_wo__depth_3__empty test__metadata_wo__depth_3__empty.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__empty)
add_test(NAME test__metadata_wo__depth_3__empty COMMAND test__metadata_wo__depth_3__empty)

add_executable(test__metadata_wo__depth_3__eq__all test__metadata_wo__depth_3__eq__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__eq__all)
add_test(NAME test__metadata_wo__depth_3__eq__all COMMAND test__metadata_wo__depth_3__eq__all)

add_executable(test__metadata_wo__depth_3__eq__unique test__metadata_wo__depth_3__eq__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__eq__unique)
add_test(NAME test__metadata_wo__depth_3__eq__unique COMMAND test__metadata_wo__depth_3__eq__unique)

add_executable(test__metadata_wo__depth_3__lt__all test__metadata_wo__depth_3__lt__all.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__lt__all)
add_test(NAME test__metadata_wo__depth_3__lt__all COMMAND test__metadata_wo__depth_3__lt__all)

add_executable(test__metadata_wo__depth_3__lt__unique test__metadata_wo__depth_3__lt__unique.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__lt__unique)
add_test(NAME test__metadata_wo__depth_3__lt__unique COMMAND test__metadata_wo__depth_3__lt__unique)

add_executable(test__metadata_wo__depth_3__merge test__metadata_wo__depth_3__merge.cpp definitions.hpp ${ctree})
configure_executable(test__metadata_wo__depth_3__merge)
add_test(NAME test__metadata_wo__depth_3__merge COMMAND test__metadata_wo__depth_3__merge)

# ========
# DEPTH 11
# ========

# WITH METADATA

add_executable(test__metadata_w__depth_11__empty test__metadata_w__depth_11__empty.cpp definitions_11.hpp ${ctree})
configure_executable(test__metadata_w__depth_11__empty)
add_test(NAME test__metadata_w__depth_11__empty COMMAND test__metadata_w__depth_11__empty)

add_executable(test__metadata_w__depth_11__eq__all test__metadata_w__depth_11__eq__all.cpp definitions_11.hpp ${ctree})
configure_executable(test__metadata_w__depth_11__eq__all)
add_test(NAME test__metadata_w__depth_11__eq__all COMMAND test__metadata_w__depth_11__eq__all)

# WITHOUT METADATA

add_executable(test__metadata_wo__depth_11__empty test__metadata_wo__depth_11__empty.cpp definitions_11.hpp ${ctree})
configure_executable(test__metadata_wo__depth_11__empty)
add_test(NAME test__metadata_wo__depth_11__empty COMMAND test__metadata_wo__depth_11__empty)

add_executable(test__metadata_wo__depth_11__eq__all test__metadata_wo__depth_11__eq__all.cpp definitions_11.hpp ${ctree})
configure_executable(test__metadata_wo__depth_11__eq__all)
add_test(NAME test__metadata_wo__depth_11__eq__all COMMAND test__metadata_wo__depth_11__eq__all)

# ===========
# MISCELLANEA
# ===========

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	# Size in bytes
	add_executable(test_bytes test_bytes.cpp ${ctree})
	configure_executable(test_bytes)
	add_test(NAME test_bytes COMMAND test_bytes)
endif()

# Search
add_executable(test_search test_search.cpp definitions.hpp ${ctree})
configure_executable(test_search)
add_test(NAME test_search COMMAND test_search)
