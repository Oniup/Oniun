#pragma once

#include "Oniun/Event/Event.h"
#include "Oniun/Renderer/Window.h"

namespace Oniun
{
    class IWindowEvent : public IEvent
    {
    public:
        IWindowEvent(Window* window)
            : m_Window(window)
        {
        }

        FORCE_INLINE Window* GetWindow()
        {
            return m_Window;
        }

        FORCE_INLINE const Window* GetWindow() const
        {
            return m_Window;
        }

    private:
        Window* m_Window;
    };

    class WindowCloseEvent : public IWindowEvent
    {
    public:
        WindowCloseEvent(Window* window)
            : IWindowEvent(window), m_IsClosing(true)
        {
        }

        FORCE_INLINE bool IsClosing() const
        {
            return m_IsClosing;
        }

        FORCE_INLINE bool ShouldCancelClose()
        {
            m_IsClosing = false;
        }

    private:
        bool m_IsClosing;
    };

    class WindowSetPositionEvent : public IWindowEvent
    {
    public:
        WindowSetPositionEvent(Window* window, int32 xPos, int32 yPos)
            : IWindowEvent(window), m_XPos(xPos), m_YPos(yPos)
        {
        }

        FORCE_INLINE int32 XPos() const
        {
            return m_XPos;
        }

        FORCE_INLINE int32 YPos() const
        {
            return m_YPos;
        }

    private:
        int32 m_XPos;
        int32 m_YPos;
    };

    class WindowSetSizeEvent : public IWindowEvent
    {
    public:
        WindowSetSizeEvent(Window* window, int32 xSize, int32 ySize)
            : IWindowEvent(window), m_XSize(xSize), m_YSize(ySize)
        {
        }

        FORCE_INLINE int32 XSize() const
        {
            return m_XSize;
        }

        FORCE_INLINE int32 YSize() const
        {
            return m_YSize;
        }

    private:
        int32 m_XSize;
        int32 m_YSize;
    };

    class WindowFocusEvent : public IWindowEvent
    {
    public:
        WindowFocusEvent(Window* window, bool isFocused)
            : IWindowEvent(window), m_IsFocused(isFocused)
        {
        }

        FORCE_INLINE bool IsFocused() const
        {
            return m_IsFocused;
        }

    private:
        bool m_IsFocused;
    };
}
