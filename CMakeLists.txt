cmake_minimum_required(VERSION 2.6)
project(Implied_Price_Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

option(BUILD_GTESTS "Builds the googletest subdirectory" ON)
option(BUILD_LTTNG "Builds the lttng trace subdirectory" OFF)

message(WARNING "CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}")

# Handle Boost here
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIBRARY_DIR})

add_subdirectory(include)
add_subdirectory(examples)
add_subdirectory(Timed_examples)
add_subdirectory(Quote_Server)
if(BUILD_LTTNG)
  add_subdirectory(LTTNG-traces)
endif(BUILD_LTTNG)
if(BUILD_GTESTS)
  add_subdirectory(gtest)
endif(BUILD_GTESTS)