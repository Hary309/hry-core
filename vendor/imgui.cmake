file(GLOB imgui-src
    imgui/*.cpp
    imgui/*.h)

add_library(imgui STATIC ${imgui-src})
target_include_directories(imgui PUBLIC imgui)