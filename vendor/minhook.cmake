file(GLOB_RECURSE minhook-src
    minhook/*.c
    minhook/*.h)

add_library(minhook STATIC ${minhook-src})

target_compile_options(minhook PUBLIC ${STATIC_FLAG})
target_include_directories(minhook PUBLIC minhook/include)