set(RENDERER_SOURCE_FILES
    vk_renderer.cpp
    vk_swapchain.h
    vk_swapchain.cpp

    external/VkBootstrap.h
    external/VkBootstrap.cpp
)

target_sources(Renderer PRIVATE ${RENDERER_SOURCE_FILES})

if(WIN32)
    add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
endif()

find_package(Vulkan REQUIRED)
target_link_libraries(Renderer PRIVATE Vulkan::Vulkan)