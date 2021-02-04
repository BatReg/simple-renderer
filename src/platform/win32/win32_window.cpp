#include "platform/window.h"

#include <string>

struct Platform::NativeWindow
{
    std::string title;
};

void Platform::Window::Init(const std::string& title)
{
    m_NativeWindow = new NativeWindow();
    m_NativeWindow->title = title;
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
