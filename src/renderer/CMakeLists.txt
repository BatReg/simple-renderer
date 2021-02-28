project(LowRenderer VERSION 0.0.1)

message(STATUS "GENERATING LOW-LEVEL RENDERER")

set(RENDERER_HEADERS
    include/renderer/renderer.h
)

set(RENDERER_SOURCE_FILES
    vulkan/vk_renderer.cpp
)

source_group("include" FILES ${RENDERER_HEADERS})
source_group("src" FILES ${RENDERER_SOURCE_FILES})

add_library(Renderer STATIC 
    ${RENDERER_HEADERS}
    ${RENDERER_SOURCE_FILES}
)

target_include_directories(Renderer PUBLIC include)

find_package(Vulkan REQUIRED)
target_link_libraries(Renderer PRIVATE Vulkan::Vulkan)

set_target_properties(Renderer PROPERTIES FOLDER "Platform Abstraction Layer")