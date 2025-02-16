#include "Oniun/Core/DebugLogger.h"

#include <cstdio>
#include <GLFW/glfw3.h>

namespace Onu
{
    DebugLogger::DebugLogger()
    {
        std::printf("Creating Debug Logger class\n");
        glfwInit();
    }

    DebugLogger::~DebugLogger()
    {
        std::printf("Now destroying Debug Logger class\n");
        glfwTerminate();
    }
}
