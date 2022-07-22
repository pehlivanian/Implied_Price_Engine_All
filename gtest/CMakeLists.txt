cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND flags "-Wall")

# Locate GTest
find_package(GTest REQUIRED)
set(Boost_DEBUG on)

# Locate Boost
find_package(Boost)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} "../include")
link_directories(${Boost_LIBRARY_DIR})

# Link runTests with what we want to test and GTest and pthread libraries

# SecPair test
add_executable(SecPair_gtest SecPair_gtest.cpp)
target_link_libraries(SecPair_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} pthread)

# QuoteEngine test
add_executable(QuoteEngine_gtest QuoteEngine_gtest.cpp)
target_link_libraries(QuoteEngine_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} boost_system pthread)
configure_file( serveme_2outrights.dat serveme_2outrights.dat COPYONLY)
configure_file( serveme_6outrights.dat serveme_6outrights.dat COPYONLY)

# Threadpool test
add_executable(ThreadPool_gtest ThreadPool_gtest.cpp)
target_link_libraries(ThreadPool_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} boost_system pthread)

# ImpliedClient  test
add_executable(ImpliedClient_gtest ImpliedClient_gtest.cpp)
target_link_libraries(ImpliedClient_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} boost_system pthread)
