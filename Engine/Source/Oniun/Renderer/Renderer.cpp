#include "Oniun/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/StringView.h"

Renderer::Renderer(const CharStringView& winTitle, int32 winWidth, int32 winHeight, WindowFlag winFlags)
    : m_Window(winTitle, winWidth, winHeight, winFlags)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        LOG(Fatal, TEXT("Failed to load glad which is required for OpenGL rendering"));
}

void Renderer::OnUpdate()
{
    if (!m_Window.IsOpen())
        Engine::Quit();

    glClearColor(0.2f, 0.5f, 0.7f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Window.PollEvents();
    m_Window.SwapBuffers();
}

Renderer::~Renderer()
{
}
