#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/RHI/IImGuiWindow.h"

namespace Oniun
{
    class RendererLayer;

    class IImGuiLayer : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        IImGuiLayer();
        ~IImGuiLayer() override;

    public:
        static const ImWchar* GetGlyphRangesRequired();

        FORCE_INLINE ImFont* GetDefaultFont() const
        {
            return m_DefaultFont;
        }

        FORCE_INLINE ImFont* GetMonoFont() const
        {
            return m_MonoFont;
        }

        FORCE_INLINE Array<IImGuiWindow*>& GetWindows()
        {
            return m_Windows;
        }

        bool Register(IImGuiWindow* window);
        void OnStart() override;

        // TODO: Replace with the config
        void SetTheme();

        virtual void NewFrame();
        virtual void RenderPlatformDrawData() = 0;

        void Render(RendererLayer& renderer);
        void UpdatePlatformWindows();

    private:
        ImFont* m_DefaultFont;
        ImFont* m_MonoFont;
        Array<IImGuiWindow*> m_Windows;
    };
}
