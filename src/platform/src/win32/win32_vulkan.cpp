#ifndef VK_USE_PLATFORM_WIN32_KHR
 #define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <platform/vulkan.h>
#include "../internal_window.h"

#include <vulkan/vulkan.h>


namespace Platform
{
    VkResult Vulkan::CreateSurface(const Window& window, const VkInstance& instance, VkSurfaceKHR* surface)
    {
        _NativeWindow* nativeWindow = (_NativeWindow*)window.m_Handle;

        VkWin32SurfaceCreateInfoKHR info{};
        info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        info.hwnd = nativeWindow->win32.handle;
        info.hinstance = nativeWindow->win32.instance;

        return vkCreateWin32SurfaceKHR(instance, &info, nullptr, surface);
    }
}
