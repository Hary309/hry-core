cmake_minimum_required(VERSION 3.4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# core
project(hry-core VERSION 0.1.0)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                             "MSVC")
  set(STATIC_FLAG /MT)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(STATIC_FLAG -static)
endif()

add_subdirectory(vendor)

file(GLOB_RECURSE hry-core-src include/*.hpp src/*.hpp src/*.cpp)
add_library(hry-core SHARED ${hry-core-src})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(hry-core PUBLIC DEBUG)
endif()

target_compile_definitions(hry-core PRIVATE HRY_EXPORT _CRT_SECURE_NO_WARNINGS)

# vscode doesn't like relative paths in errors :/
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(hry-core PUBLIC -fdiagnostics-absolute-paths)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                             "GNU")
  target_compile_options(
    hry-core
    PUBLIC -Wnon-virtual-dtor
           -Wunused
           -Woverloaded-virtual
           -Wnull-dereference
           -Wdouble-promotion
           -Wformat=2
           -Wmisleading-indentation
           -Wmost
           -Wparentheses
           -Wswitch
           -Wswitch-bool
           -Wextra
           -Wpedantic)
endif()

target_compile_options(hry-core PUBLIC ${STATIC_FLAG})

target_include_directories(hry-core PUBLIC include vendor/scs_sdk_1_10/include
                                           vendor/json/include vendor/imgui)
target_include_directories(hry-core PRIVATE src)

target_link_libraries(hry-core PUBLIC fmt::fmt-header-only)
target_link_libraries(hry-core PRIVATE imgui glad minhook)

# example
project(hry-example VERSION 0.1.0)
add_library(hry-example SHARED example/example.cpp)
target_link_libraries(hry-example PRIVATE hry-core)

# sample
project(hry-sample VERSION 0.1.0)
add_library(hry-sample SHARED example/sample.cpp)
target_link_libraries(hry-sample PRIVATE hry-core)
