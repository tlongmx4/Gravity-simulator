cmake_minimum_required(VERSION 4.3)
project(gravity)

set(CMAKE_CXX_STANDARD 14)

find_package(raylib REQUIRED)

add_executable(gravity
        gravity.cpp
)

target_link_libraries(gravity PRIVATE raylib)
