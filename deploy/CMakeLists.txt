function(target_link_hry_core target-name)

    # don't link fmt
    target_compile_definitions(${target-name} PRIVATE FMT_HEADER_ONLY)

    find_library(hry-core-debug hry-core vendor/hry-core/lib/debug)
    find_library(hry-core-release hry-core vendor/hry-core/lib/release)

    # for some reason CMAKE_BUILD_TYPE is empty for me :/
    target_link_libraries(${target-name} PRIVATE debug ${hry-core-debug})
    target_link_libraries(${target-name} PRIVATE optimized ${hry-core-release})

    target_include_directories(${target-name} PRIVATE src vendor/hry-core/include)

endfunction(target_link_hry_core)

macro(use_static_linking)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endmacro(use_static_linking)
