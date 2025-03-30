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

        // TODO: Expand so there doesn't have to be a previous array
        enum State
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
        void ProcessKey(State& current, State& previous, KeyCode key);
        bool IsRepeating(State current, State previous, int32 key) const;

    private:
        static Input* m_Instance;

        Window* m_Window;
        FixedArray<State, (uint64)KeyCode::MaxCount> m_CurrentKeyCodes;
        FixedArray<State, (uint64)KeyCode::MaxCount> m_PreviousKeyCodes;
        uint64 m_GamesConnectedCount;
    };
}
