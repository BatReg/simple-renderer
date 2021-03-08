#include "platform/window.h"
#include "renderer/renderer.h"

int main()
{
    Platform::Window wnd{};
    Renderer::Renderer renderer{};

    Platform::Rect windowRect{};
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.width = 800;
    windowRect.height = 600;

    Platform::CreateWindowInfo windowCreateInfo{};
    windowCreateInfo.title = "Super Awesome Title";
    windowCreateInfo.rect = windowRect;
    windowCreateInfo.rendererAPI = Platform::RendererAPI::Vulkan;

    wnd.Init(windowCreateInfo);

    renderer.Init();

    bool isRunning = true;
    while (isRunning)
    {
        isRunning = wnd.PollEvents();
    }

    renderer.Destroy();
}