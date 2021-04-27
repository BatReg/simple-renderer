#include <renderer/renderer.h>

#include <platform/vulkan.h>
#include "vk_core.h"
#include "vk_initializers.h"
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
        void InitSwapChain() noexcept;
        void InitCommands() noexcept;
        void InitDefaultRenderPass() noexcept;
        void InitFramebuffers() noexcept;

        bool                        m_IsInitalized{ false };

        const Platform::Window&     m_Window;

        VkInstance                  m_Instance{ nullptr };
        VkDebugUtilsMessengerEXT    m_DebugMessenger{ nullptr };
        VkDevice                    m_Device{ nullptr };
        VkPhysicalDevice            m_PhysicalDevice{ nullptr };
        VkSurfaceKHR                m_Surface{ nullptr };
        VkQueue                     m_GraphicsQueue{ nullptr };
        uint32_t                    m_GraphicsQueueFamily{};

        Vulkan::SwapChain           m_SwapChain{};

        VkCommandPool               m_CommandPool{ nullptr };
        VkCommandBuffer             m_CommandBuffer{ nullptr };
        VkRenderPass                m_RenderPass{ nullptr };
        std::vector<VkFramebuffer>  m_Framebuffers{ nullptr };
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
        InitSwapChain();
        InitCommands();
        InitDefaultRenderPass();
        InitFramebuffers();

        m_IsInitalized = true;
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy() noexcept
    {
        if(!m_IsInitalized)
        {
            return;
        }

        for(size_t i = 0; i < m_Framebuffers.size(); ++i)
        {
            vkDestroyFramebuffer(m_Device, m_Framebuffers[i], nullptr);
        }

        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
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

    void Renderer::NativeRenderer::InitSwapChain() noexcept
    {
        Vulkan::SwapChainInitInfo swapChainInitInfo = {};
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
        VkCommandPoolCreateInfo commandPoolInfo = Vulkan::Init::CommandPoolCreateInfo(
            m_GraphicsQueueFamily, 
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        );

        VK_CHECK_RESULT(vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_CommandPool));

        VkCommandBufferAllocateInfo cmdAllocInfo = Vulkan::Init::CommandBufferAllocateInfo(
            m_CommandPool,
            1,
            VK_COMMAND_BUFFER_LEVEL_PRIMARY
        );

        VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &m_CommandBuffer));
    }

    void Renderer::NativeRenderer::InitDefaultRenderPass() noexcept
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format          = m_SwapChain.GetColorFormat();
        colorAttachment.samples         = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment   = 0;
        colorAttachmentRef.layout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount    = 1;
        subpass.pColorAttachments       = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext            = nullptr;
        renderPassInfo.attachmentCount  = 1;
        renderPassInfo.pAttachments     = &colorAttachment;
        renderPassInfo.subpassCount     = 1;
        renderPassInfo.pSubpasses       = &subpass;

        VK_CHECK_RESULT(vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass));
    }

    void Renderer::NativeRenderer::InitFramebuffers() noexcept
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext           = nullptr;
        framebufferInfo.renderPass      = m_RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.width           = 800;
        framebufferInfo.height          = 600;
        framebufferInfo.layers          = 1;

        const size_t swapChainImageCount = m_SwapChain.GetImageCount();
        m_Framebuffers = std::vector<VkFramebuffer>(swapChainImageCount);

        const std::vector<VkImageView> imageViews = m_SwapChain.GetImageViews();
        for(size_t i = 0; i < swapChainImageCount; ++i)
        {
            framebufferInfo.pAttachments = &imageViews[i];
            VK_CHECK_RESULT(vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffers[i]));
        }
    }
}
