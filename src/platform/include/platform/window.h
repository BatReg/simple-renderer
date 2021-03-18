#pragma once

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
        Window() noexcept;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window() noexcept;

        void        Init(const CreateWindowInfo& info) noexcept;
        bool        ShouldClose() const noexcept;
        std::string GetTitle() const noexcept;
        void        PollEvents() const noexcept;

    private:
        struct NativeWindow;

        NativeWindow* m_Handle;
    };
};