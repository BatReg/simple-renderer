#include <renderer/renderer.h>

#include <platform/vulkan.h>
#include "vk_core.h"
#include "vk_swapchain.h"

#include <vulkan/vulkan.h>
#include "external/VkBootstrap.h"

#include <iostream>

namespace Renderer
{
    class Renderer::NativeRenderer
    {
    public:
        NativeRenderer(const Platform::Window& window) noexcept;

        void Init() noexcept;
        void Destroy() noexcept;

    private:
        void InitVulkan() noexcept;
        void InitSwapchain() noexcept;
        void InitCommands() noexcept;

        bool                        m_IsInitalized{ false };

        const Platform::Window&     m_Window;

        VkInstance                  m_Instance;
        VkDebugUtilsMessengerEXT    m_DebugMessenger;
        VkDevice                    m_Device;
        VkPhysicalDevice            m_PhysicalDevice;
        VkSurfaceKHR                m_Surface;
        VkQueue                     m_GraphicsQueue;
        uint32_t                    m_GraphicsQueueFamily;

        Vulkan::SwapChain           m_SwapChain{};

        VkCommandPool               m_CommandPool;
        VkCommandBuffer             m_CommandBuffer;
    };

    Renderer::Renderer(const Platform::Window& window) noexcept :
        m_Window(window),
        m_NativeRenderer(std::make_unique<NativeRenderer>(window)) {}

    Renderer::~Renderer() noexcept
    {
        m_NativeRenderer.reset();
    }

    void Renderer::Init() noexcept
    {
        m_NativeRenderer->Init();
    }

    void Renderer::Destroy() noexcept
    {
        m_NativeRenderer->Destroy();
    }

    Renderer::NativeRenderer::NativeRenderer(const Platform::Window& window) noexcept
        : m_Window(window) {}

    void Renderer::NativeRenderer::Init() noexcept
    {
        InitVulkan();
        InitSwapchain();
        InitCommands();

        m_IsInitalized = true;
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy() noexcept
    {
        if(!m_IsInitalized)
        {
            return;
        }

        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

        m_SwapChain.Destroy();

        vkDestroyDevice(m_Device, nullptr);
        vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
        std::cout << "Vulkan Renderer destroyed" << std::endl;
    }

    void Renderer::NativeRenderer::InitVulkan() noexcept
    {
        vkb::InstanceBuilder instanceBuilder;
        auto instanceRet = instanceBuilder
            .set_app_name("Test")
            .request_validation_layers(true)
            .require_api_version(1, 1, 0)
            .use_default_debug_messenger()
            .build();

        vkb::Instance vkbInstance = instanceRet.value();

        m_Instance          = vkbInstance.instance;
        m_DebugMessenger    = vkbInstance.debug_messenger;

        VK_CHECK_RESULT(Platform::Vulkan::CreateSurface(m_Window, m_Instance, &m_Surface));

        vkb::PhysicalDeviceSelector selector{ vkbInstance };
        vkb::PhysicalDevice physicalDevice = selector
            .set_minimum_version(1, 1)
            .set_surface(m_Surface)
            .select()
            .value();

        std::cout << "GPU: " << physicalDevice.properties.deviceName << std::endl;

        vkb::DeviceBuilder deviceBuilder{ physicalDevice };
        vkb::Device vkbDevice = deviceBuilder.build().value();

        m_PhysicalDevice        = physicalDevice.physical_device;
        m_Device                = vkbDevice.device;
        m_GraphicsQueue         = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        m_GraphicsQueueFamily   = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    }

    void Renderer::NativeRenderer::InitSwapchain() noexcept
    {
        Vulkan::SwapChainInitInfo swapChainInitInfo{};
        swapChainInitInfo.instance          = m_Instance;
        swapChainInitInfo.physicalDevice    = m_PhysicalDevice;
        swapChainInitInfo.device            = m_Device;
        swapChainInitInfo.surface           = m_Surface;

        m_SwapChain.Init(swapChainInitInfo);

        // TODO (Nikita): implement passing window's width and height
        m_SwapChain.CreateSwapChain(800, 600, true);
    }

    void Renderer::NativeRenderer::InitCommands() noexcept
    {
        VkCommandPoolCreateInfo commandPoolInfo{};
        commandPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.pNext               = nullptr;
        commandPoolInfo.queueFamilyIndex    = m_GraphicsQueueFamily;
        commandPoolInfo.flags               = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VK_CHECK_RESULT(vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_CommandPool));

        VkCommandBufferAllocateInfo cmdAllocInfo{};
        cmdAllocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdAllocInfo.pNext              = nullptr;
        cmdAllocInfo.commandPool        = m_CommandPool;
        cmdAllocInfo.commandBufferCount = 1;
        cmdAllocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &m_CommandBuffer));
    }
}
