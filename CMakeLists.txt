cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tool_box VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CLI11 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(tool_box
    src/main.cpp
    src/hash.cpp
    src/commands/hash_command.cpp
)

target_include_directories(tool_box PRIVATE include)

target_link_libraries(tool_box PRIVATE
    CLI11::CLI11
    fmt::fmt
    nlohmann_json::nlohmann_json
    bcrypt
)

# ---- Tests ----
enable_testing()
find_package(Catch2 3 CONFIG REQUIRED)

add_executable(tests_hash
    tests/test_hash.cpp
    src/hash.cpp
)

target_include_directories(tests_hash PRIVATE include)
target_link_libraries(tests_hash PRIVATE
    Catch2::Catch2WithMain
    bcrypt
)

include(Catch)
catch_discover_tests(tests_hash)