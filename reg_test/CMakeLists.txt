cmake_minimum_required(VERSION 2.6)

set(SOURCES ImpliedEngine_test)

include_directories("../include")

FOREACH(source ${SOURCES})
    add_executable(${source} ${source}.cpp)
    target_link_libraries(${source} LINK_PUBLIC IElib pthread boost_thread boost_system)
ENDFOREACH(source)

