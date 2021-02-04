#include "platform/window.h"

#include <string>

struct Window
{
    std::string title;
};

Window* InitWindow(const char* title)
{
    Window* wnd = new Window();
    wnd->title = title;
    return wnd;
}

const char* GetWindowTitle(Window* window)
{
    return window->title.c_str();
}
