#pragma once

#include "Oniun/Core/BaseTypes.h"

// Forward declare
struct GLFWwindow;
struct GLFWmonitor;

namespace Oniun
{
    using WindowFlags = int32;
    enum WindowFlags_
    {
        WindowFlags_None = 0,
        WindowFlags_Resizable = 1 << 0,
        WindowFlags_TransparentFrameBuffer = 1 << 1,
        WindowFlags_Borderless = 1 << 2,
        WindowFlags_ModeFullscreen = 1 << 3,
        WindowFlags_ModeWindowed = 1 << 4,
    };

    typedef void(*PfnWindowDpiChange)(float xScale, float yScale);

    class Window
    {
    public:
        static constexpr WindowFlags DefaultFlags = WindowFlags_Resizable;

    public:
        Window();
        Window(const StringView& title, int32 width, int32 height, WindowFlags flags = DefaultFlags);
        ~Window();

    public:
        FORCE_INLINE GLFWwindow* GetInternalWindow()
        {
            return m_Window;
        }

        FORCE_INLINE const GLFWwindow* GetInternalWindow() const
        {
            return m_Window;
        }

        FORCE_INLINE WindowFlags GetFlags() const
        {
            return m_Flags;
        }

    public:
        void PollEvents();
        void SwapBuffers();
        bool IsOpen() const;
        void ShouldClose(bool shouldClose);

        int32 GetWidth() const;
        int32 GetHeight() const;
        void GetSize(int32* width, int32* height) const;

        int32 GetXPosition() const;
        int32 GetYPosition() const;
        void GetPosition(int32* x, int32* y) const;

        StringView Title() const;

        GLFWmonitor* GetInternalCurrentMonitor() const;

    private:
        void SetupCallbacks();

    private:
        GLFWwindow* m_Window;
        WindowFlags m_Flags;
    };
}
