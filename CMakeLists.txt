cmake_minimum_required(VERSION 3.21)

project(tool_box VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CLI11 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(tool_box src/main.cpp)

target_link_libraries(tool_box PRIVATE
    CLI11::CLI11
    fmt::fmt
)
