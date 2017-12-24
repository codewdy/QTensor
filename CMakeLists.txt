cmake_minimum_required(VERSION 2.8)
project(QTensor)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")
include(packages)
include(utils)

include_directories(.)
#Flags
if (DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -DDEBUG")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -DDEBUG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DNDEBUG -O2")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DNDEBUG -O2")
endif()

# LIBRARY
new_library(core qtensor/core)
new_library(framework qtensor/framework)

# EXECUTABLE

# TESTS
enable_testing()
new_test(test_core test/core)
new_test(test_framework test/framework)
