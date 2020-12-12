file(GLOB imgui-src imgui/*.cpp imgui/*.h)

add_library(imgui STATIC ${imgui-src})

target_link_libraries(imgui PRIVATE glad)
target_include_directories(imgui PUBLIC imgui)
