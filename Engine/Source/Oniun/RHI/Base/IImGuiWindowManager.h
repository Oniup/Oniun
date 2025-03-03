#pragma once

#include "Oniun/Core/Templates/Array.h"
#include "Oniun/RHI/IImGuiWindow.h"

class Renderer;

class IImGuiWindowManager
{
    Array<IImGuiWindow*> m_Windows;

public:
    IImGuiWindowManager();
    virtual ~IImGuiWindowManager()
    {
    }

    bool Add(IImGuiWindow* window);

    virtual void Initialize(Renderer& renderer);
    virtual void Terminate();
    virtual void NewFrame();
    virtual void RenderPlatformDrawData() = 0;

    void Render(Renderer& renderer);
    void UpdatePlatformWindows();
};
