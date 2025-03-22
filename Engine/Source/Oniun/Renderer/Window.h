#pragma once

#include "Oniun/Core/BaseTypes.h"

// Forward declare
struct GLFWwindow;
struct GLFWmonitor;

namespace Oniun
{
    enum WindowFlag
    {
        WindowFlag_None = 0,
        WindowFlag_ResizableBit = 1 << 0,
        WindowFlag_TransparentFrameBufferBit = 1 << 1,
        WindowFlag_BorderlessBit = 1 << 2,
        WindowFlag_ModeFullscreenBit = 1 << 3,
        WindowFlag_ModeWindowedBit = 1 << 4,
    };

    typedef void(*PfnWindowDpiChange)(float xScale, float yScale);

    class Window
    {
    public:
        using Flags = int32;

        static constexpr Flags DefaultFlags = WindowFlag_ResizableBit;

    public:
        Window();
        Window(const StringView& title, int32 width, int32 height, Flags flags = DefaultFlags);
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
        GLFWwindow* m_Window;
        Flags m_Flags;
    };
}
