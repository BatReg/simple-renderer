#pragma once

#include <string>

namespace Platform
{
    struct NativeWindow;

    class Window
    {
    public:
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void        Init(const std::string& title);
        std::string GetTitle() const;
        void        Destroy();

    private:
        NativeWindow* m_NativeWindow = nullptr;
    };
};