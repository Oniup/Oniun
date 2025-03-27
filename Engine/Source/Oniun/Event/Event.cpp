#include "Oniun/Event/Event.h"

namespace Oniun
{
    EventDispatcher* EventDispatcher::m_Instance = nullptr;

    EventDispatcher::EventDispatcher()
    {
        m_Instance = this;
    }

    EventDispatcher::~EventDispatcher()
    {
        m_Instance = nullptr;
    }

    bool EventDispatcher::AddCallback(UUID id, const OnEventCallback& callback)
    {
        if (!m_RegisteredEvents.Contains(id))
            m_RegisteredEvents.Add(id);
        Array<OnEventCallback>& callbacks = m_RegisteredEvents.At(id);
        for (OnEventCallback& registered : callbacks)
        {
            if (registered == callback)
            {
                LOG(Warning, "Cannot add duplicate event callback");
                return false;
            }
        }
        callbacks.Add(callback);
        return true;
    }

    bool EventDispatcher::RemoveCallback(UUID id, const OnEventCallback& callback)
    {
        Array<OnEventCallback>* callbacks = m_RegisteredEvents.Try(id);
        if (!callbacks)
            return false;
        for (OnEventCallback& registered : *callbacks)
        {
            if (registered == callback)
            {
                callbacks->Remove(registered);
                return true;
            }
        }
        return false;
    }

    void EventDispatcher::FireEventCallbacks(UUID id, IEvent* event, void* sender)
    {
        Array<OnEventCallback>* callbacks = m_RegisteredEvents.Try(id);
        if (!callbacks)
            return;

        for (OnEventCallback& registered : *callbacks)
            registered(event, sender);
    }
}
