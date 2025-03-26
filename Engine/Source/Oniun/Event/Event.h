#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/UUID.h"

namespace Oniun
{
    class Event
    {
        friend class EventDispatcher;

    public:
        Event()
            : m_Handled(false)
        {
        }

    private:
        bool m_Handled;
    };

    /// @brief Called when an event is fired
    ///
    /// @param event  Current event that is fired from sender
    /// @param sender Instance that fired the event. May be nullptr
    using OnEventCallback = Function<void(Event* event, void* sender)>;

    class EventDispatcher : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        EventDispatcher();
        ~EventDispatcher() override;

    public:
        template <typename TEvent, typename... TArgs>
        static bool AddListener(OnEventCallback&& callback);

        template <typename TEvent>
        static void RemoveListener(const OnEventCallback& callback);

        template <typename TEvent>
        static void FireEvent(TEvent& event, void* sender = nullptr);

    private:
        bool AddCallback(UUID id, OnEventCallback&& callback);
        bool RemoveCallback(UUID id, const OnEventCallback& callback);
        void FireEventCallbacks(UUID id, Event* event, void* sender);

    private:
        static EventDispatcher* m_Instance;

        UUID m_EventTypeHandle;
        HashMap<UUID, Array<OnEventCallback>> m_RegisteredEvents;
    };

    template <typename TEvent, typename ... TArgs>
    bool EventDispatcher::AddListener(OnEventCallback&& callback)
    {
        UUID id = TypeInfo::GetFastId<TEvent>();
        bool result = m_Instance->AddCallback(id, Memory::Move(callback));
        if (!result)
            LOG(Error, "Failed to add listener to event \"{}\"", TypeInfo::GetInfo<TEvent>().Name);
        return result;
    }

    template <typename TEvent>
    void EventDispatcher::RemoveListener(const OnEventCallback& callback)
    {
        UUID id = TypeInfo::GetFastId<TEvent>();
#ifndef NDEBUG
        if (!m_Instance->RemoveCallback(id, callback))
            LOG(Warning, "No event type \"{}\" callback matches given callback", TypeInfo::GetInfo<TEvent>().Name);
#else
        RemoveCallback(id, callback);
#endif
    }

    template <typename TEvent>
    void EventDispatcher::FireEvent(TEvent& event, void* sender)
    {
        UUID id = TypeInfo::GetFastId<TEvent>();
        m_Instance->FireEventCallbacks(id, &event, sender);
    }
}
