#pragma once

#include <memory>
#include <string>

namespace Platform
{
    struct Rect
    {
        int x{};
        int y{};
        int width{};
        int height{};
    };

    enum class RendererAPI
    {
        None,
        OpenGL,
        Vulkan
    };

    struct CreateWindowInfo
    {
        std::string title{};
        Rect        rect{};
        RendererAPI rendererAPI{};
    };

    class Window
    {
    public:
        Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window();

        void        Init(const CreateWindowInfo& info);
        std::string GetTitle() const;
        bool        PollEvents() const;

    private:
        struct NativeWindow;

        std::unique_ptr<NativeWindow> m_NativeWindow;
    };
};