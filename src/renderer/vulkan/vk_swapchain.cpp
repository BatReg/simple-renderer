#include "vk_swapchain.h"
#include "vk_core.h"

#include "external/VkBootstrap.h"

namespace Renderer::Vulkan
{
    void SwapChain::Init(const SwapChainInitInfo& info) noexcept
    {
        m_Instance          = info.instance;
        m_PhysicalDevice    = info.physicalDevice;
        m_Device            = info.device;
        m_Surface           = info.surface;
    }

    void SwapChain::CreateSwapChain(const uint32_t& width, const uint32_t& height, bool vSync) noexcept
    {
        vkb::SwapchainBuilder swapchainBuilder{ m_PhysicalDevice, m_Device, m_Surface };
        VkPresentModeKHR presentMode = static_cast<VkPresentModeKHR>(vSync * VK_PRESENT_MODE_FIFO_KHR + !vSync * VK_PRESENT_MODE_IMMEDIATE_KHR);
        vkb::Swapchain vkbSwapchain = swapchainBuilder
            .use_default_format_selection()
            .set_desired_present_mode(presentMode)
            .set_desired_extent(width, height)
            .build()
            .value();

        m_SwapChain     = vkbSwapchain.swapchain;
        m_Images        = vkbSwapchain.get_images().value();
        m_ImageViews    = vkbSwapchain.get_image_views().value();
        m_ColorFormat   = vkbSwapchain.image_format;
    }

    void SwapChain::Destroy() noexcept
    {
        if(m_SwapChain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
        }

        for(size_t i = 0; i < m_ImageViews.size(); ++i)
        {
            vkDestroyImageView(m_Device, m_ImageViews[i], nullptr);
        }

        if(m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        }
    }
}
