#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Renderer/Window.h"

class Renderer : public EngineLayer
{
    API_ENGINE_LAYER()

private:
    Window m_Window;

public:
    Renderer(const CharStringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags = Window::DefaultFlags);
    ~Renderer() override;

    void OnUpdate() override;
};
