cmake_minimum_required(VERSION 3.13)
project(BallsOnThreads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Curses REQUIRED)
include_directories(/usr/include)

add_executable(BallsOnThreads main.cpp Ball.cpp Ball.h DirectionGenerator.cpp DirectionGenerator.h Line.cpp Line.h)
target_link_libraries(BallsOnThreads ${CURSES_LIBRARIES})