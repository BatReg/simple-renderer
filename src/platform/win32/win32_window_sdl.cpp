#include "platform/window.h"

#include <SDL.h>

#include <string>
#include <iostream>

struct Platform::Window::NativeWindow
{
    std::string title{};
    SDL_Window* pSDLWindow{ nullptr };
};

Platform::Window::Window() : m_NativeWindow(std::make_unique<NativeWindow>()){}

Platform::Window::~Window()
{
    if (m_NativeWindow->pSDLWindow) 
    {
        SDL_DestroyWindow(m_NativeWindow->pSDLWindow);
    }
    m_NativeWindow.reset();
}

void Platform::Window::Init(const CreateWindowInfo& info)
{
    if (m_NativeWindow->pSDLWindow) return;

    SDL_Init(SDL_INIT_VIDEO);

    const SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(
        (info.rendererAPI == RendererAPI::OpenGL) * SDL_WINDOW_OPENGL |
        (info.rendererAPI == RendererAPI::Vulkan) * SDL_WINDOW_VULKAN);

    const int x = (info.rect.x == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.x;
    const int y = (info.rect.y == 0) * SDL_WINDOWPOS_UNDEFINED + info.rect.y;

    SDL_Window* window = SDL_CreateWindow(info.title.c_str(), x, y, info.rect.width, info.rect.height, windowFlags);

    m_NativeWindow = std::make_unique<NativeWindow>();
    m_NativeWindow->pSDLWindow = window;
    m_NativeWindow->title = info.title;
}

std::string Platform::Window::GetTitle() const
{
    return m_NativeWindow->title;
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
