#pragma once

#include <platform/window.h>
#include <vulkan/vulkan.h>

#include <vector>

struct SwapChainBuffer
{
    VkImage     image;
    VkImageView view;
};

class VulkanSwapchain
{
public:
    VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept;

    VulkanSwapchain() = delete;
    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

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
