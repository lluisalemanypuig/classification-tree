cmake_minimum_required(VERSION 3.16)

# project declaration
project(EXAMPLES DESCRIPTION "Examples for classifiation tree")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# MAKE EXECUTABLES

file(GLOB_RECURSE ctree ../ctree/*.hpp)

add_executable(example example.cpp ${ctree})
configure_executable(example)
