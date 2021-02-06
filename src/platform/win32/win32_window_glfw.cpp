#include "platform/window.h"

#include <GLFW/glfw3.h>

#include <string>

struct Platform::Window::NativeWindow
{
    std::string title{};
    GLFWwindow* window{nullptr};
};

void Platform::Window::Init(const CreateWindowInfo& info)
{
    if (m_NativeWindow != nullptr) return;

    m_NativeWindow = new NativeWindow();

    glfwInit();

    const int glfwGraphicsAPI = (info.rendererAPI == RendererAPI::OpenGL) * GLFW_OPENGL_API |
        (info.rendererAPI == RendererAPI::Vulkan) * GLFW_NO_API;

    glfwWindowHint(GLFW_CLIENT_API, glfwGraphicsAPI);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(info.rect.width, info.rect.height, info.title.c_str(), nullptr, nullptr);

    m_NativeWindow->title = info.title;
    m_NativeWindow->window = window;
}

std::string Platform::Window::GetTitle() const
{
    return m_NativeWindow->title;
}

bool Platform::Window::PollEvents() const
{
    glfwPollEvents();

    return !glfwWindowShouldClose(m_NativeWindow->window);
}

void Platform::Window::Destroy()
{
    glfwDestroyWindow(m_NativeWindow->window);
    glfwTerminate();

    delete m_NativeWindow;
    m_NativeWindow = nullptr;
}
