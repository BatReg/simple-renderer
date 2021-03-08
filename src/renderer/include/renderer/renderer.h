#pragma once

#include <memory>

namespace Renderer
{
    class Renderer
    {
    public:
        Renderer();
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        ~Renderer();

        void Init();
        void Destroy();

    private:
        class NativeRenderer;

        std::unique_ptr<NativeRenderer> m_NativeRenderer;
    };
}