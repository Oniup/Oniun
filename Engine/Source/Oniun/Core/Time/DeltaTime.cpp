#include "Oniun.pch.h"
#include "Oniun/Core/Time/DeltaTime.h"

#include <GLFW/glfw3.h>

namespace Oniun
{
    void DeltaTime::CalcDelta()
    {
        float time = GetElapsedTime();
        Instance()->m_Delta = time - Instance()->m_Last;
        Instance()->m_Last = time;
    }

    float DeltaTime::GetElapsedTime()
    {
        return static_cast<float>(glfwGetTime());
    }

    DeltaTime::DeltaTime()
        : m_Last(static_cast<float>(glfwGetTime())), m_Delta(0.0f)
    {
    }
}
