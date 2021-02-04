#include <Windows.h>

#include "platform/window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    Platform::Window wnd{};
    wnd.Init("Super Awesome Title");

    MessageBoxA(nullptr, "Hello World!", wnd.GetTitle().c_str(), MB_OK | MB_ICONINFORMATION);

    wnd.Destroy();
}