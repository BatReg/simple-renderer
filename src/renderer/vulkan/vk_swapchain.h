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
        VkInstance                      m_Instance{ nullptr };
        VkDevice                        m_Device{ nullptr };
        VkPhysicalDevice                m_PhysicalDevice{ nullptr };
        VkSurfaceKHR                    m_Surface{ nullptr };
        VkFormat                        m_ColorFormat{};
        VkSwapchainKHR                  m_SwapChain{ nullptr };
        std::vector<VkImage>            m_Images{};
        std::vector<VkImageView>        m_ImageViews{};
    };
}
