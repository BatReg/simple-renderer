#include "platform/window.h"
#include "../internal_window.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <string>
#include <iostream>

namespace Platform
{
    Window::Window() noexcept
    {
        _NativeWindow* nativeWindow = new _NativeWindow();

        m_Handle = reinterpret_cast<NativeWindow*>(nativeWindow);
    }

    Window::~Window() noexcept
    {
        _NativeWindow* nativeWindow = (_NativeWindow*)m_Handle;

        if (nativeWindow->win32.pSDLWindow)
        {
            SDL_DestroyWindow(nativeWindow->win32.pSDLWindow);
        }

        delete nativeWindow;
    }

    void Window::Init(const CreateWindowInfo& info) noexcept
    {
        _NativeWindow* nativeWindow = (_NativeWindow*)m_Handle;
        if (nativeWindow->win32.pSDLWindow) return;

        SDL_Init(SDL_INIT_VIDEO);

        const SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(
            (info.rendererAPI == RendererAPI::OpenGL) * SDL_WINDOW_OPENGL |
            (info.rendererAPI == RendererAPI::Vulkan) * SDL_WINDOW_VULKAN);

        const int x = (info.rect.x == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.x;
        const int y = (info.rect.y == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.y;

        SDL_Window* window = SDL_CreateWindow(info.title.c_str(), x, y, info.rect.width, info.rect.height, windowFlags);

        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(window, &wmInfo);
        HWND hwnd = wmInfo.info.win.window;

        nativeWindow->win32.pSDLWindow = window;
        nativeWindow->title = info.title;
        nativeWindow->win32.handle = hwnd;
    }

    std::string Window::GetTitle() const noexcept
    {
        _NativeWindow* nativeWindow = (_NativeWindow*)m_Handle;
        return nativeWindow->title;
    }

    bool Window::PollEvents() const noexcept
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return false;
        }

        return true;
    }
}
