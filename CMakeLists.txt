cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

# core
project(hry-core VERSION 0.1.0)

add_subdirectory(vendor)

file(GLOB_RECURSE hry-core-src
    include/*.hpp
    src/*.hpp
    src/*.cpp)
add_library(hry-core SHARED ${hry-core-src})

target_include_directories(hry-core PRIVATE include/hry)
target_include_directories(hry-core PRIVATE src)

target_link_libraries(hry-core PUBLIC imgui)

# example
project(hry-example VERSION 0.1.0)
add_library(hry-example SHARED example/example.cpp)
target_include_directories(hry-example PRIVATE include)
target_link_libraries(hry-example PRIVATE hry-core)
