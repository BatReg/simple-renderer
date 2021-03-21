#include "vk_swapchain.h"
#include "vk_core.h"

#include <platform/vulkan.h>

namespace Renderer::Vulkan
{
    void SwapChain::Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept
    {
        m_Instance = instance;
        m_PhysicalDevice = physicalDevice;
        m_Device = device;
    }

    void SwapChain::CreateSurface(const Platform::Window& window) noexcept
    {
        VkResult r = Platform::Vulkan::CreateSurface(window, m_Instance, &m_Surface);
        VK_CHECK_RESULT(r);

        std::cout << "Create Surface: " << (r == VK_SUCCESS) << std::endl;
    }

    void SwapChain::CreateSwapchain(const uint32_t& width, const uint32_t& height, bool vSync) noexcept
    {
    }

    void SwapChain::Dispose() noexcept
    {
        if(m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }
    }
}
