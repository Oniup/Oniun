#include "Oniun/Renderer/RendererLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/StringView.h"

namespace Oniun
{
    RendererLayer::RendererLayer(const StringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags)
        : m_Window(winTitle, winWidth, winHeight, winFlags), m_ImGuiLayer(nullptr)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            LOG(Fatal, "Failed to load glad which is required for OpenGL rendering");
    }

    RendererLayer::~RendererLayer()
    {
        m_ImGuiLayer = nullptr;
    }

    void RendererLayer::SetImGuiWindowLayer(ImGuiLayer* layer)
    {
        DEBUG_ASSERT(!m_ImGuiLayer && "Cannot have two ImGui contexts");
        m_ImGuiLayer = layer;
    }

    void RendererLayer::OnUpdate()
    {
        if (!m_Window.IsOpen())
            Engine::Quit();

        m_Window.PollEvents();

        if (m_ImGuiLayer)
            m_ImGuiLayer->Render(*this);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_ImGuiLayer)
        {
            m_ImGuiLayer->RenderPlatformDrawData();
            m_ImGuiLayer->UpdatePlatformWindows();
        }

        m_Window.SwapBuffers();
    }
}
