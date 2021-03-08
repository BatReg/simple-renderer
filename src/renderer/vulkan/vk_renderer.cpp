#include "renderer/renderer.h"

#include <vulkan/vulkan.h>
#include "external/VkBootstrap.h"

#include <iostream>

namespace Renderer
{
    class Renderer::NativeRenderer
    {
    public:
        void Init();
        void Destroy();

    private:
        void InitVulkan();

        bool m_isInitalized;

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkDevice m_Device;
        VkPhysicalDevice m_PhysicalDevice;
    };

    Renderer::Renderer() : m_NativeRenderer(std::make_unique<NativeRenderer>()) {}

    Renderer::~Renderer()
    {
        m_NativeRenderer.reset();
    }

    void Renderer::Init()
    {
        m_NativeRenderer->Init();
    }

    void Renderer::Destroy()
    {
        m_NativeRenderer->Destroy();
    }

    void Renderer::NativeRenderer::Init()
    {
        InitVulkan();
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy()
    {
        vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
        std::cout << "Vulkan Renderer destroyed" << std::endl;
    }

    void Renderer::NativeRenderer::InitVulkan()
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
    }
}