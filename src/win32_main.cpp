#include <Windows.h>

#include "platform/window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    Window* wnd = InitWindow("Super awesome title!");

    MessageBoxA(nullptr, "Hello World!", GetWindowTitle(wnd), MB_OK | MB_ICONINFORMATION);
}