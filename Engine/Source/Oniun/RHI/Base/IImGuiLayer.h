#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/RHI/IImGuiWindow.h"

class RendererLayer;

class IImGuiLayer : public EngineLayer
{
public:
    IImGuiLayer();
    ~IImGuiLayer() override;

public:
    bool Add(IImGuiWindow* window);

    void OnStart() override;

    virtual void NewFrame();
    virtual void RenderPlatformDrawData() = 0;

    void Render(RendererLayer& renderer);
    void UpdatePlatformWindows();

private:
    Array<IImGuiWindow*> m_Windows;
};
