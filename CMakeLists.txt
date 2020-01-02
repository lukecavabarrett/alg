cmake_minimum_required(VERSION 3.1)
project(alg)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)

#set(SOURCE_FILES main.cpp)
#add_executable(calendar_run ${SOURCE_FILES})

#include_directories(calendars)

#add_subdirectory(calendars)
add_subdirectory(tests)
#add_subdirectory(calendars_boost_tests)

#target_link_libraries(calendar_run calendars)
