#pragma once

#include <SDL.h>
#include <Windows.h>

namespace Platform
{
    #define NATIVE_STATE_WINDOW _NativeWindowWin32 win32

    struct _NativeWindowWin32
    {
        SDL_Window* pSDLWindow{ nullptr };
        HWND        handle{};
    };
}