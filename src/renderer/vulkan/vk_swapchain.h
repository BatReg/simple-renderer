#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Renderer::Vulkan
{
    struct SwapChainInitInfo
    {
        VkInstance          instance;
        VkPhysicalDevice    physicalDevice;
        VkDevice            device;
        VkSurfaceKHR        surface;
    };

    class SwapChain
    {
    public:
        SwapChain() = default;
        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        void Init(const SwapChainInitInfo& info) noexcept;
        void CreateSwapChain(const uint32_t& width, const uint32_t& height, bool vSync = false) noexcept;
        void Destroy() noexcept;

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
        VkSwapchainKHR                  m_SwapChain;
        std::vector<VkImage>            m_Images;
        std::vector<VkImageView>        m_ImageViews;
    };
}
