#include "platform/window.h"

#include <SDL.h>

#include <string>

struct Platform::NativeWindow
{
    std::string title{};
    SDL_Window* pSDLWindow{ nullptr };
};

void Platform::Window::Init(const CreateWindowInfo& info)
{
    m_NativeWindow = new NativeWindow();
    m_NativeWindow->title = info.title;

    SDL_Init(SDL_INIT_VIDEO);
    const SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(
        (info.rendererAPI == RendererAPI::OpenGL) * SDL_WINDOW_OPENGL |
        (info.rendererAPI == RendererAPI::Vulkan) * SDL_WINDOW_VULKAN);

    const int x = (info.rect.x == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.x;
    const int y = (info.rect.y == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.y;

    m_NativeWindow->pSDLWindow = SDL_CreateWindow(info.title.c_str(), x, y, info.rect.width, info.rect.height, windowFlags);
}

std::string Platform::Window::GetTitle() const
{
    return m_NativeWindow->title;
}

void Platform::Window::Destroy()
{
    delete m_NativeWindow;
    m_NativeWindow = nullptr;
}

bool Platform::Window::PollEvents() const
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) return false;
    }

    return true;
}
