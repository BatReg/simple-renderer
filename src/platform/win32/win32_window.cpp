#include "platform/window.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <string>
#include <iostream>
#include <memory>

namespace Platform
{
    struct Window::NativeWindow
    {
        std::string title{};
        SDL_Window* pSDLWindow{ nullptr };
        HWND        handle{};
    };

    Window::Window() noexcept : m_NativeWindow(std::make_unique<NativeWindow>()) {}

    Window::~Window() noexcept
    {
        if (m_NativeWindow->pSDLWindow)
        {
            SDL_DestroyWindow(m_NativeWindow->pSDLWindow);
        }
        m_NativeWindow.reset();
    }

    void Window::Init(const CreateWindowInfo& info) noexcept
    {
        if (m_NativeWindow->pSDLWindow) return;

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

        m_NativeWindow = std::make_unique<NativeWindow>();
        m_NativeWindow->pSDLWindow = window;
        m_NativeWindow->title = info.title;
        m_NativeWindow->handle = hwnd;
    }

    std::string Window::GetTitle() const noexcept
    {
        return m_NativeWindow->title;
    }

    void* Window::GetNativeHandle() const noexcept
    {
        return &(m_NativeWindow->handle);
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
