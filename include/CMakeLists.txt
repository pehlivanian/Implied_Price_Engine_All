cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/utils)

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(IElib STATIC ${SRC_FILES})
target_include_directories(IElib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
