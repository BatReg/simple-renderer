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
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void        Init(const CreateWindowInfo& info);
        std::string GetTitle() const;
        void        Destroy();
        bool        PollEvents() const;

    private:
        struct NativeWindow;

        NativeWindow* m_NativeWindow{ nullptr };
    };
};