cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

# Locate GTest
find_package(GTest REQUIRED)
# set(Boost_DEBUG on)
# find_package(Boost)
# include_directories(${Boost_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} "../include")

#Link runTests with what we want to test and GTest and pthread libraries

# SecPair test
add_executable(SecPair_gtest SecPair_gtest.cpp)
target_link_libraries(SecPair_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} pthread)

# QuoteEngine test
add_executable(QuoteEngine_gtest QuoteEngine_gtest.cpp)
target_link_libraries(QuoteEngine_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} pthread)
configure_file( serveme.dat serveme.dat COPYONLY)