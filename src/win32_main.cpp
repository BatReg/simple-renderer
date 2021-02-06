#include "platform/window.h"

int main()
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
    windowCreateInfo.rendererAPI = Platform::RendererAPI::OpenGL;

    wnd.Init(windowCreateInfo);

    while(wnd.PollEvents())
    {
        
    }

    wnd.Destroy();
}