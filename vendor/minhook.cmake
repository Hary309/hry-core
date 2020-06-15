file(GLOB_RECURSE minhook-src
    minhook/*.c
    minhook/*.h)

add_library(minhook STATIC ${minhook-src})
target_include_directories(minhook PUBLIC minhook/include)
