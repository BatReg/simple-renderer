#include "vk_swapchain.h"
#include "vk_core.h"

#include <platform/vulkan.h>

namespace Renderer::Vulkan
{
    void SwapChain::Init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept
    {
        m_Instance = instance;
        m_PhysicalDevice = physicalDevice;
        m_Device = device;
    }

    void SwapChain::CreateSurface(const Platform::Window& window) noexcept
    {
        VK_CHECK_RESULT(Platform::Vulkan::CreateSurface(window, m_Instance, &m_Surface));
    }

    void SwapChain::CreateSwapChain(const uint32_t& width, const uint32_t& height, bool vSync) noexcept
    {
    }

    void SwapChain::Destroy() noexcept
    {
        if(m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }
    }
}
