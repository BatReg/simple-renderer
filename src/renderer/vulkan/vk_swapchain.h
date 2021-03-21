#pragma once

#include <platform/window.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace Renderer::Vulkan
{
    struct SwapChainBuffer
    {
        VkImage     image;
        VkImageView view;
    };

    class SwapChain
    {
    public:
        SwapChain() = default;
        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        void Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept;
        void CreateSurface(const Platform::Window& window) noexcept;
        void CreateSwapchain(const uint32_t& width, const uint32_t& height, bool vSync = false) noexcept;
        void Dispose() noexcept;

        VkSurfaceKHR GetSurface() const noexcept
        {
            return m_Surface;
        }

    private:
        VkInstance                      m_Instance;
        VkDevice                        m_Device;
        VkPhysicalDevice                m_PhysicalDevice;
        VkSurfaceKHR                    m_Surface;
        VkFormat                        m_ColorFormat;
        VkColorSpaceKHR                 m_ColorSpace;
        VkSwapchainKHR                  m_SwapChain = VK_NULL_HANDLE;

        uint32_t                        m_ImageCount;
        uint32_t                        m_QueueNodeIndex = UINT32_MAX;
        std::vector<VkImage>            m_Images;
        std::vector<SwapChainBuffer>    m_Buffers;
    };
}
