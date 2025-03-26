#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/KeyCode.h"
#include "Oniun/Event/Event.h"

namespace Oniun
{
    class Window;

    class Input : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        static constexpr uint64 MaxGamepadsConnectedCount = 16;

        enum KeyState
        {
            Released = 0,
            Pressed,
        };

    public:
        Input();
        ~Input() override;

        void OnStart() override;
        void OnUpdate() override;

    private:
        void ProcessKey(KeyState& current, KeyState& previous, KeyCode key);

        bool IsRepeating(KeyState current, KeyState previous, int32 key) const;

    private:
        static Input* m_Instance;

        Window* m_Window;
        FixedArray<KeyState, (uint64)KeyCode::MaxCount> m_CurrentKeyCodes;
        FixedArray<KeyState, (uint64)KeyCode::MaxCount> m_PreviousKeyCodes;
        uint64 m_GamesConnectedCount;
    };
}
