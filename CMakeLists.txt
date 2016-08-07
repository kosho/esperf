cmake_minimum_required(VERSION 2.8)
project(esperf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.cpp)
add_executable(esperf ${SOURCE_FILES})
target_link_libraries(esperf curl)