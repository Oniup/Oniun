#include "Oniun/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Event/KeyboardEvents.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/Renderer/Window.h"

namespace Oniun
{
    Input* Input::m_Instance = nullptr;

    Input::Input()
        : m_Window(nullptr)
    {
        m_Instance = this;
    }

    Input::~Input()
    {
        m_Instance = nullptr;
        m_Window = nullptr;
    }

    void Input::OnStart()
    {
        m_Window = Engine::GetLayer<RendererLayer>()->GetWindow();
        for (uint64 i = 0; i < (uint64)KeyCode::MaxCount; ++i)
        {
            m_CurrentKeyCodes[i] = Released;
            m_PreviousKeyCodes[i] = Released;
        }
    }

    void Input::OnUpdate()
    {
        for (uint64 i = 0; i < (uint64)KeyCode::MaxCount; ++i)
            ProcessKey(m_CurrentKeyCodes[i], m_PreviousKeyCodes[i], (KeyCode)i);

        // TODO: Mouse
        // TODO: Gamepad
    }

    void Input::ProcessKey(KeyState& current, KeyState& previous, KeyCode key)
    {
        current = (KeyState)glfwGetKey(m_Window->GetInternalWindow(), (int32)key);
        bool isRepeating = IsRepeating(current, previous, (int32)key);

        if (current == Pressed)
        {
            KeyPressedEvent event(key, isRepeating);
            EventDispatcher::FireEvent(event);
        }
        else if (current == Released && previous == Pressed)
        {
            KeyReleasedEvent event(key);
            EventDispatcher::FireEvent(event);
        }
        previous = current;
    }

    bool Input::IsRepeating(KeyState current, KeyState previous, int32 key) const
    {
        if (current == previous)
        {
            if (current)
                return true;
        }
        return false;
    }
}
