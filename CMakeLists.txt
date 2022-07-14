cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(hry-core VERSION 1.0.7)

add_subdirectory(vendor)

# core
file(GLOB_RECURSE hry-core-src include/*.hpp src/*.hpp src/*.cpp)
add_library(hry-core SHARED ${hry-core-src})

target_compile_definitions(hry-core PUBLIC _USE_MATH_DEFINES)
target_compile_definitions(hry-core PRIVATE HRY_EXPORT _CRT_SECURE_NO_WARNINGS)

# vscode doesn't like relative paths in errors :/
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(hry-core PUBLIC -fdiagnostics-absolute-paths)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(
    hry-core
    PUBLIC -Wnon-virtual-dtor
           -Wunused
           -Woverloaded-virtual
           -Wnull-dereference
           -Wdouble-promotion
           -Wformat=2
           -Wmisleading-indentation
           -Wparentheses
           -Wswitch
           -Wswitch-bool
           -Wextra
           -Wpedantic)
endif()

target_include_directories(hry-core PUBLIC include vendor/json/include
                                           vendor/imgui)
target_include_directories(hry-core PRIVATE src vendor/scs_sdk/include
                                            vendor/imgui_markdown)

target_link_libraries(hry-core PUBLIC fmt::fmt-header-only)
target_link_libraries(hry-core PRIVATE imgui glad minhook)
