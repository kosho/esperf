cmake_minimum_required(VERSION 2.8)
project(esperf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Worker.cpp Worker.h Options.cpp Options.h Stats.cpp Stats.h Timer.cpp Timer.h Esperf.cpp Esperf.h)
add_executable(esperf ${SOURCE_FILES})
target_link_libraries(esperf curl)