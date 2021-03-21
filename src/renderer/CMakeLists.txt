project(LowRenderer VERSION 0.0.1)

message(STATUS "GENERATING LOW-LEVEL RENDERER")

add_library(Renderer STATIC include/renderer/renderer.h)

target_include_directories(Renderer PUBLIC include)
target_link_libraries(Renderer PRIVATE Platform)

add_subdirectory(vulkan)

set_target_properties(Renderer PROPERTIES FOLDER "Platform Abstraction Layer")