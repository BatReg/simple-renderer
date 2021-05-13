#pragma once

#include "platform/window.h"

#include <memory>

namespace Renderer
{
    class Renderer
    {
    public:
        Renderer(const Platform::Window&) noexcept;
        Renderer(const Renderer&) = delete;
        Renderer &operator=(const Renderer&) = delete;

        ~Renderer() noexcept;

        void Init() noexcept;
        void Destroy() noexcept;
        void Render(int frameNumber) noexcept;

    private:
        class NativeRenderer;

        const Platform::Window&         m_Window;
        std::unique_ptr<NativeRenderer> m_NativeRenderer;
    };
}
