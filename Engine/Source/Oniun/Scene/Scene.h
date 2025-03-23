#pragma once

#include "Oniun/Core/UUID.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Scene/ComponentPool.h"
#include "Oniun/Scene/EntityEntry.h"

namespace Oniun
{
    class Entity;

    class Scene
    {
        friend Entity;

    public:
        Scene(const StringView& title = "Empty Scene");
        ~Scene();

    public:
        FORCE_INLINE const String& GetTitle() const
        {
            return m_Title;
        }

        FORCE_INLINE HashMap<UUID, EntityEntry>& GetEntityEntries()
        {
            return m_Entities;
        }

        FORCE_INLINE const HashMap<UUID, EntityEntry>& GetEntityEntries() const
        {
            return m_Entities;
        }

        FORCE_INLINE HashMap<uint64, ComponentPool>& GetComponentPools()
        {
            return m_Pools;
        }

        FORCE_INLINE const HashMap<uint64, ComponentPool>& GetComponentPools() const
        {
            return m_Pools;
        }

        Entity Add(const StringView& name = "Entity");
        Entity Find(const EntityEntry& name);
        Entity Find(UUID entityId);
        void RenameEntity(Entity& entity);
        void Remove(const Entity& entity);
        bool IsAlive(const Entity& entity);

        template <typename TComponent, typename... TArgs>
        TComponent* AddComponent(UUID entity, TArgs&&... args);

        template <typename TComponent>
        TComponent* GetComponent(UUID entity);

    private:
        byte* AddComponent(UUID entity, const ComponentType& type);
        byte* GetComponent(UUID entity, const ComponentType& type);

    private:
        String m_Title;

        HashMap<UUID, EntityEntry> m_Entities;
        HashMap<uint64, ComponentPool> m_Pools;
    };

    template <typename TComponent, typename ... TArgs>
    TComponent* Scene::AddComponent(UUID entity, TArgs&&... args)
    {
        constexpr ComponentType type = ComponentType::Get<TComponent>();
        byte* byteData = AddComponent(entity, type);
        if (byteData)
        {
            TComponent* component = reinterpret_cast<TComponent*>(byteData);
            Memory::ConstructItem(component, Memory::Forward<TArgs>(args)...);
            return component;
        }
        return nullptr;
    }

    template <typename TComponent>
    TComponent* Scene::GetComponent(UUID entity)
    {
        constexpr ComponentType type = ComponentType::Get<TComponent>();
        byte* byteData = GetComponent(entity, type);
        if (byteData)
            return reinterpret_cast<TComponent*>(byteData);
        return nullptr;
    }
}
