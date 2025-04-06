#include "Oniun/RHI/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Oniun/Core/Logger.h"

namespace Oniun::RHI
{
    Context::Context()
    {
    }

    Context::~Context()
    {
    }

    void Context::Initialize()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            LOG(Fatal, "Failed to load glad which is required for OpenGL rendering");
    }

    void Context::SetWireframeMode(bool enable)
    {
        if (enable)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
