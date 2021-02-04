#pragma once

namespace Platform
{
    struct Window;

    Window* InitWindow(const char* title);
    const char* GetWindowTitle(Window* window);
    void DestroyWindow(Window* window);
};