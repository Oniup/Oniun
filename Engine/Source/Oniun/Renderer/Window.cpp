#include "Oniun/Renderer/Window.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/StringView.h"

#include <GLFW/glfw3.h>

namespace Oniun
{
    Window::Window()
        : m_Window(nullptr), m_Flags(WindowFlag_None)
    {
    }

    Window::Window(const StringView& title, int32 width, int32 height, Flags flags)
        : m_Window(nullptr), m_Flags(flags)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if ONU_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        glfwWindowHint(GLFW_RESIZABLE, flags & WindowFlag_ResizableBit ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, flags & WindowFlag_TransparentFrameBufferBit ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, flags & WindowFlag_BorderlessBit ? GLFW_FALSE : GLFW_TRUE);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

        bool setSize = width == -1 || height == -1;
        if (flags & WindowFlag_ModeFullscreenBit || (setSize && flags & WindowFlag_BorderlessBit))
        {
            width = vidMode->width;
            height = vidMode->height;
        }
        else if (setSize)
        {
            width = vidMode->width / 2;
            height = vidMode->height / 2;
        }

        m_Window = glfwCreateWindow(width, height, *title, flags & WindowFlag_ModeFullscreenBit ? monitor : nullptr, nullptr);
        if (!m_Window)
            LOG(Fatal, "Failed to create window");
        glfwMakeContextCurrent(m_Window);

        int32 centerX = (vidMode->width - width) / 2;
        int32 centerY = (vidMode->height - height) / 2;
        glfwSetWindowPos(m_Window, centerX, centerY);
    }

    Window::~Window()
    {
        if (m_Window)
            glfwDestroyWindow(m_Window);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    bool Window::IsOpen() const
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void Window::ShouldClose(bool shouldClose)
    {
        glfwSetWindowShouldClose(m_Window, shouldClose);
    }

    int32 Window::GetWidth() const
    {
        int32 width;
        glfwGetFramebufferSize(m_Window, &width, nullptr);
        return width;
    }

    int32 Window::GetHeight() const
    {
        int32 height;
        glfwGetFramebufferSize(m_Window, nullptr, &height);
        return height;
    }

    void Window::GetSize(int32* width, int32* height) const
    {
        glfwGetFramebufferSize(m_Window, width, height);
    }

    int32 Window::GetXPosition() const
    {
        int32 x;
        glfwGetWindowPos(m_Window, &x, nullptr);
        return x;
    }

    int32 Window::GetYPosition() const
    {
        int32 y;
        glfwGetWindowPos(m_Window, nullptr, &y);
        return y;
    }

    void Window::GetPosition(int32* x, int32* y) const
    {
        glfwGetWindowPos(m_Window, x, y);
    }

    StringView Window::Title() const
    {
        return glfwGetWindowTitle(m_Window);
    }

    GLFWmonitor* Window::GetInternalCurrentMonitor() const
    {
        int32 xPos, yPos;
        GetPosition(&xPos, &yPos);

        int32 count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        for (int32 i = 0; i < count; ++i)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
            int32 mX, mY;
            glfwGetMonitorPos(monitors[i], &mX, &mY);
            if (xPos >= mX && xPos < (mX + mode->width))
            {
                if (yPos >= mY && yPos < (mY + mode->height))
                {
                    return monitors[i];
                }
            }
        }
        return nullptr;
    }
}
