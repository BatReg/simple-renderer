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
        Window() noexcept;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window() noexcept;

        void        Init(const CreateWindowInfo& info) noexcept;
        std::string GetTitle() const noexcept;
        void*       GetNativeHandle() const noexcept;
        bool        PollEvents() const noexcept;

    private:
        struct NativeWindow;

        std::unique_ptr<NativeWindow> m_NativeWindow;
    };
};