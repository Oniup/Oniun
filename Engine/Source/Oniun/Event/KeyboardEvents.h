#pragma once

#include "Oniun/Core/KeyCode.h"
#include "Oniun/Event/Event.h"

namespace Oniun
{
    class KeyCodeEvent : public Event
    {
    public:
        KeyCodeEvent(KeyCode code)
            : m_KeyCode(code)
        {
        }

        FORCE_INLINE KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

    protected:
        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyCodeEvent
    {
    public:
        KeyPressedEvent(KeyCode code, bool repeat)
            : KeyCodeEvent(code), m_Repeat(repeat)
        {
        }

        FORCE_INLINE bool IsRepeated() const
        {
            return m_Repeat;
        }

    private:
        bool m_Repeat;
    };

    class KeyReleasedEvent : public KeyCodeEvent
    {
    public:
        KeyReleasedEvent(KeyCode code)
            : KeyCodeEvent(code)
        {
        }
    };
}
