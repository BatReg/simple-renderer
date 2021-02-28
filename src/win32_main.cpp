#include "platform/window.h"
#include "renderer/renderer.h"

#include <Windows.h>

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Platform::Window wnd{};

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

    bool isRunning = true;
    while (isRunning)
    {
        isRunning = wnd.PollEvents();
    }
}