#include "renderer/renderer.h"

#include <iostream>

struct Renderer::Material
{
};

struct Renderer::Mesh
{
};

void Renderer::Renderer::Init()
{
    std::cout << "Renderer initialized" << std::endl;
}

void Renderer::Renderer::Destroy()
{
    std::cout << "Renderer destroyed" << std::endl;
}