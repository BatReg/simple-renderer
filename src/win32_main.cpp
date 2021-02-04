#include <Windows.h>

#include "platform/window.h"

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    Platform::Window wnd{};
    wnd.Init("Super Awesome Title");

    while(wnd.PollEvents())
    {
        
    }

    wnd.Destroy();
}