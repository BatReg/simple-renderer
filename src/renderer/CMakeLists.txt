project(LowRenderer VERSION 0.0.1)

message(STATUS "GENERATING LOW-LEVEL RENDERER")

set(RENDERER_HEADERS include/renderer/renderer.h)

add_library(Renderer STATIC ${RENDERER_HEADERS})

target_include_directories(Renderer PUBLIC include)
target_link_libraries(Renderer PRIVATE Platform)

add_subdirectory(vulkan)

set_target_properties(Renderer PROPERTIES FOLDER "Platform Abstraction Layer")