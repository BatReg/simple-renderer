#pragma once

#ifdef PLATFORM_WIN32
 #include "win32/win32_platform.h"
#else
 #error Unsupported platform!
#endif

#include <string>

namespace Platform
{
    struct _NativeWindow
    {
        std::string title{};
        bool        shouldClose{};

        NATIVE_WINDOW_STATE;
    };
}
