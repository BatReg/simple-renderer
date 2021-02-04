#include <Windows.h>

#include "platform/window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    Platform::Window* wnd = Platform::InitWindow("Super awesome title!");

    MessageBoxA(nullptr, "Hello World!", Platform::GetWindowTitle(wnd), MB_OK | MB_ICONINFORMATION);

    Platform::DestroyWindow(wnd);
}