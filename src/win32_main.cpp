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

    MessageBoxA(nullptr, "Hello World!", wnd.GetTitle().c_str(), MB_OK | MB_ICONINFORMATION);

    wnd.Destroy();
}