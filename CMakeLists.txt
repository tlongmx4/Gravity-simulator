cmake_minimum_required(VERSION 4.3)
project(gravity)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(gravity
        gravity.cpp
)

target_link_libraries(gravity PRIVATE raylib)

add_custom_command(TARGET gravity POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/ephemeris.csv $<TARGET_FILE_DIR:gravity>)
