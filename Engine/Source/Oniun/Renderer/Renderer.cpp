#include "Oniun/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/StringView.h"

Renderer::Renderer(const StringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags)
    : m_Window(winTitle, winWidth, winHeight, winFlags)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        LOG(Fatal, "Failed to load glad which is required for OpenGL rendering");
}

Renderer::~Renderer()
{
    m_EditorGui.Terminate();
}

void Renderer::OnStart()
{
    m_EditorGui.Initialize(*this);
}

void Renderer::OnUpdate()
{
    if (!m_Window.IsOpen())
        Engine::Quit();

    m_Window.PollEvents();


    m_EditorGui.Render(*this);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_EditorGui.RenderPlatformDrawData();
    m_EditorGui.UpdatePlatformWindows();

    m_Window.SwapBuffers();
}
