#include <renderer/renderer.h>
#include "vk_swapchain.h"

#include <platform/vulkan.h>
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

        bool                        m_isInitalized{ false };

        const Platform::Window&     m_Window;

        VkInstance                  m_Instance;
        VkDebugUtilsMessengerEXT    m_DebugMessenger;
        VkDevice                    m_Device;
        VkPhysicalDevice            m_PhysicalDevice;

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
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy() noexcept
    {
        m_SwapChain.Dispose();
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

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        VkPhysicalDevice device = devices[0];

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        std::cout << deviceProperties.deviceName << std::endl;

        m_SwapChain.Connect(m_Instance, m_PhysicalDevice, m_Device);
        m_SwapChain.CreateSurface(m_Window);
    }
}
