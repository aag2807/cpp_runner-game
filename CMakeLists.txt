cmake_minimum_required(VERSION 3.27)
project(Dasher)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib 4.5.0 REQUIRED)

add_executable(Dasher main.cpp
        src/dimension.cpp)

target_link_libraries(Dasher raylib)
