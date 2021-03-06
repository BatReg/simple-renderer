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
        void Render(int frameNumber) noexcept;

    private:
        void InitVulkan() noexcept;
        void InitSwapChain() noexcept;
        void InitCommands() noexcept;
        void InitDefaultRenderPass() noexcept;
        void InitFramebuffers() noexcept;
        void InitSyncStructures() noexcept;

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

        VkSemaphore                 m_PresentSemaphore{ nullptr };
        VkSemaphore                 m_RenderSemaphore{ nullptr };
        VkFence                     m_RenderFence{ nullptr };
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

    void Renderer::Render(int frameNumber) noexcept
    {
        m_NativeRenderer->Render(frameNumber);
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
        InitSyncStructures();

        m_IsInitalized = true;
        std::cout << "Vulkan Renderer initialized" << std::endl;
    }

    void Renderer::NativeRenderer::Destroy() noexcept
    {
        if(!m_IsInitalized)
        {
            return;
        }

        vkDeviceWaitIdle(m_Device);

        vkDestroySemaphore(m_Device, m_RenderSemaphore, nullptr);
        vkDestroySemaphore(m_Device, m_PresentSemaphore, nullptr);

        vkDestroyFence(m_Device, m_RenderFence, nullptr);

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

        const std::vector<Vulkan::SwapChainBuffer> buffers = m_SwapChain.GetBuffers();
        for(size_t i = 0; i < swapChainImageCount; ++i)
        {
            framebufferInfo.pAttachments = &buffers[i].view;
            VK_CHECK_RESULT(vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffers[i]));
        }
    }

    void Renderer::NativeRenderer::InitSyncStructures() noexcept
    {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK_RESULT(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_RenderFence));

        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;

        VK_CHECK_RESULT(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentSemaphore));
        VK_CHECK_RESULT(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderSemaphore));
    }

    void Renderer::NativeRenderer::Render(int frameNumber) noexcept
    {
        VK_CHECK_RESULT(vkWaitForFences(m_Device, 1, &m_RenderFence, true, UINT64_MAX));
        VK_CHECK_RESULT(vkResetFences(m_Device, 1, &m_RenderFence));

        uint32_t swapchainImageIndex{};
        m_SwapChain.AcquireNextImage(m_PresentSemaphore, &swapchainImageIndex);

        VK_CHECK_RESULT(vkResetCommandBuffer(m_CommandBuffer, 0));

        VkCommandBuffer cmd = m_CommandBuffer;

        VkCommandBufferBeginInfo cmdBeginInfo = {};
        cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBeginInfo.pNext = nullptr;
        cmdBeginInfo.pInheritanceInfo = nullptr;
        cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK_RESULT(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        float flash = abs(sin(frameNumber / 120.0f));
        VkClearValue clearValue;
        clearValue.color = { {0.0f, 0.0f, flash, 1.0f} };

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext               = nullptr;
        renderPassBeginInfo.renderPass          = m_RenderPass;
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent   = {800, 600};
        renderPassBeginInfo.framebuffer         = m_Framebuffers[swapchainImageIndex];
        renderPassBeginInfo.clearValueCount     = 1;
        renderPassBeginInfo.pClearValues        = &clearValue;

        vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(cmd);
        VK_CHECK_RESULT(vkEndCommandBuffer(cmd));

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext                = nullptr;
        submitInfo.pWaitDstStageMask    = &waitStage;
        submitInfo.waitSemaphoreCount   = 1;
        submitInfo.pWaitSemaphores      = &m_PresentSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = &m_RenderSemaphore;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &cmd;

        VK_CHECK_RESULT(vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_RenderFence));

        m_SwapChain.QueuePresent(m_GraphicsQueue, swapchainImageIndex, m_RenderSemaphore);
    }
}
