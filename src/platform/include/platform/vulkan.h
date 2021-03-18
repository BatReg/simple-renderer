#pragma once

#include <platform/window.h>

#include <vulkan/vulkan.h>

namespace Platform
{
    class Vulkan
    {
    public:
        static VkResult CreateSurface(const Window& window, const VkInstance& instance, VkSurfaceKHR* surface);
    };
}
