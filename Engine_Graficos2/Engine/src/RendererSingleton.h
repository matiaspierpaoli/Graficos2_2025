#pragma once
#include "Renderer/Renderer.h"

class RendererSingleton
{
public:
    RendererSingleton(const RendererSingleton&) = delete;
    static RendererSingleton& Get()
    {
        static RendererSingleton instance;
        return instance;
    }
    static Renderer* GetRenderer() { return Get().GetRendererInternal(); }
    static void SetRenderer(Renderer* newRenderer) { Get().SetRendererInternal(newRenderer); }
private:
    Renderer* GetRendererInternal() { return renderer; }
    void SetRendererInternal(Renderer* newRenderer) { renderer = newRenderer; }
    RendererSingleton() {}
    Renderer* renderer;
};
