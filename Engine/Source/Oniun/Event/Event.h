#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/UUID.h"

namespace Oniun
{
    class IEvent
    {
        friend class EventDispatcher;

    public:
        IEvent()
            : m_Handled(false)
        {
        }

    private:
        bool m_Handled;
    };

    /// @brief Called when an event is fired.
    ///
    /// @param event  Current event that is fired from sender.
    /// @param sender Instance that fired the event. May be nullptr.
    using OnEventCallback = Function<void(IEvent* event, void* sender)>;

    class EventDispatcher : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        EventDispatcher();
        ~EventDispatcher() override;

    public:
        /// @brief Adds a listener for a specific event type. The callback will be executed when the event is fired by
        ///        the sender.
        ///
        /// @tparam TEvent  The type of event to listen for.
        /// @tparam TArgs   Additional arguments for the event.
        /// @param callback The callback function to be called when the event is fired.
        /// @return True if the listener was successfully added.
        template <typename TEvent, typename... TArgs>
        static bool AddListener(const OnEventCallback& callback);

        /// @brief Removes a listener for a specific event type.
        ///
        /// @tparam TEvent  The type of event to remove the listener from.
        /// @param callback The callback function to be removed.
        template <typename TEvent>
        static void RemoveListener(const OnEventCallback& callback);

        /// @brief Fires an event to all listeners callbacks.
        ///
        /// @tparam TEvent The type of event to fire.
        /// @param event   The event instance to fire.
        /// @param sender  The instance that fired the event. May be nullptr.
        template <typename TEvent>
        static void FireEvent(TEvent& event, void* sender = nullptr);

    private:
        bool AddCallback(UUID id, const OnEventCallback& callback);
        bool RemoveCallback(UUID id, const OnEventCallback& callback);
        void FireEventCallbacks(UUID id, IEvent* event, void* sender);

    private:
        /// Singleton instance that is set when added to engine layer
        static EventDispatcher* m_Instance;

        UUID m_EventTypeHandle;
        HashMap<UUID, Array<OnEventCallback>> m_RegisteredEvents;
    };

    template <typename TEvent, typename ... TArgs>
    bool EventDispatcher::AddListener(const OnEventCallback& callback)
    {
        UUID id = TypeInfo::GetFastId<TEvent>();
        bool result = m_Instance->AddCallback(id, callback);
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
