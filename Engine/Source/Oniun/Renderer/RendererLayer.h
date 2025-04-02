#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Renderer/Window.h"
#include "Oniun/RHI/ImGuiLayer.h"

namespace Oniun
{
    class RendererLayer : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        RendererLayer(const StringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags = Window::DefaultFlags);
        ~RendererLayer() override;

    public:
        FORCE_INLINE Window* GetWindow()
        {
            return &m_Window;
        }

        FORCE_INLINE const Window* GetWindow() const
        {
            return &m_Window;
        }

        FORCE_INLINE ImGuiLayer* GetImGuiWindowManager()
        {
            return m_ImGuiLayer;
        }

        FORCE_INLINE const ImGuiLayer* GetImGuiWindowManager() const
        {
            return m_ImGuiLayer;
        }

    public:
        void SetImGuiWindowLayer(ImGuiLayer* layer);

        void OnUpdate() override;

    private:
        Window m_Window;
        ImGuiLayer* m_ImGuiLayer;
    };
}
