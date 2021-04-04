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

        bool                        m_isInitalized{ false };

        const Platform::Window&     m_Window;

        VkInstance                  m_Instance;
        VkDebugUtilsMessengerEXT    m_DebugMessenger;
        VkDevice                    m_Device;
        VkPhysicalDevice            m_PhysicalDevice;
        VkSurfaceKHR                m_Surface;

        Vulkan::SwapChain           m_SwapChain{};
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
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy() noexcept
    {
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

        m_Instance = vkbInstance.instance;
        m_DebugMessenger = vkbInstance.debug_messenger;

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

        m_PhysicalDevice = physicalDevice.physical_device;
        m_Device = vkbDevice.device;
    }

    void Renderer::NativeRenderer::InitSwapchain() noexcept
    {
        Vulkan::SwapChainInitInfo swapChainInitInfo{};
        swapChainInitInfo.instance = m_Instance;
        swapChainInitInfo.physicalDevice = m_PhysicalDevice;
        swapChainInitInfo.device = m_Device;
        swapChainInitInfo.surface = m_Surface;
        
        m_SwapChain.Init(swapChainInitInfo);

        // TODO (Nikita): implement passing window's width and height
        m_SwapChain.CreateSwapChain(800, 600, true);
    }
}
