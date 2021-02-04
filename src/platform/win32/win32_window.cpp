#include "platform/window.h"

#include <string>

struct Platform::Window
{
    std::string title;
};

Platform::Window* Platform::InitWindow(const char* title)
{
    Window* wnd = new Window();
    wnd->title = title;
    return wnd;
}

const char* Platform::GetWindowTitle(Window* window)
{
    return window->title.c_str();
}

void Platform::DestroyWindow(Window* window)
{
    delete window;
}
