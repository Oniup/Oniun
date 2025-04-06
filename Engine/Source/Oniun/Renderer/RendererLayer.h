#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Renderer/Window.h"
#include "Oniun/RHI/Context.h"
#include "Oniun/RHI/ImGuiLayer.h"

#include "Oniun/RHI/Shader.h"
#include "Oniun/RHI/Texture.h"
#include "Oniun/RHI/VertexBuffer.h"

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

        void OnStart() override;
        void OnUpdate() override;

    private:
        RHI::Context m_GraphicsContext;
        Window m_Window;
        ImGuiLayer* m_ImGuiLayer;

        uint32 m_Framebuffer;
        uint32 m_Renderbuffer;
        RHI::Texture m_FramebufferTexture;
        RHI::Shader m_Shader;
        RHI::Texture m_Texture;
        RHI::VertexBuffer m_VertexBuffer;
    };
}
