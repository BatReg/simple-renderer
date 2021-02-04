#include "platform/window.h"

#include <SDL.h>

#include <string>

struct Platform::NativeWindow
{
    std::string title{};
    SDL_Window* pSDLWindow{ nullptr };
};

void Platform::Window::Init(const std::string& title)
{
    m_NativeWindow = new NativeWindow();
    m_NativeWindow->title = title;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL);

    m_NativeWindow->pSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, windowFlags);
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
