target_sources(Renderer PRIVATE vk_core.h
                                vk_initializers.h
                                vk_initializers.cpp
                                vk_renderer.cpp
                                vk_swapchain.h
                                vk_swapchain.cpp

                                external/VkBootstrap.h
                                external/VkBootstrap.cpp)

if(WIN32)
    add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
endif()

find_package(Vulkan REQUIRED)
target_link_libraries(Renderer PRIVATE Vulkan::Vulkan)