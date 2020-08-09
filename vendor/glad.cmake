file(GLOB glad-src glad/src/glad.c glad/include/glad/glad.h)

add_library(glad STATIC ${glad-src})

target_compile_options(glad PUBLIC ${STATIC_FLAG})
target_include_directories(glad PUBLIC glad/include)
