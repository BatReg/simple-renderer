#include "platform/window.h"
#include "renderer/renderer.h"

int main()
{
    Platform::Window wnd{};
    Renderer::Renderer renderer{wnd};

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

    while (!wnd.ShouldClose())
    {
        wnd.PollEvents();
        renderer.Render();
    }

    renderer.Destroy();
}