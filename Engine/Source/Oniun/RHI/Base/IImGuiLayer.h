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
    static const ImWchar* GetGlyphRangesRequired();

    bool Add(IImGuiWindow* window);
    void OnStart() override;

    FORCE_INLINE ImFont* GetDefaultFont() const
    {
        return m_DefaultFont;
    }

    FORCE_INLINE ImFont* GetMonoFont() const
    {
        return m_MonoFont;
    }

    virtual void NewFrame();
    virtual void RenderPlatformDrawData() = 0;

    void Render(RendererLayer& renderer);
    void UpdatePlatformWindows();

private:
    ImFont* m_DefaultFont;
    ImFont* m_MonoFont;
    Array<IImGuiWindow*> m_Windows;
};
