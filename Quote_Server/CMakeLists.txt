cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

set(SOURCES QuoteServer QuoteClient)

FOREACH(source ${SOURCES})
    add_executable(${source} ${source}.cpp)
ENDFOREACH(source)

