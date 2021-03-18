#pragma once

#include <SDL.h>
#include <Windows.h>

#define NATIVE_WINDOW_STATE _NativeWindowWin32 win32

namespace Platform
{
    struct _NativeWindowWin32
    {
        SDL_Window* pSDLWindow{ nullptr };
        HWND        handle;
        HINSTANCE   instance;
    };
}