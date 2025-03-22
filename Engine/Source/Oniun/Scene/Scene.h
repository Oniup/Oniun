#pragma once

#include "Oniun/Core/UUID.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Scene/ComponentPool.h"

namespace Oniun
{
    class Entity;

    class Scene
    {
    public:
        static constexpr uint64 MaxEntityNameSize = 30;
        static constexpr uint64 MaxEntityFullNameSize = MaxEntityNameSize + 32;

    private:
        struct EntityEntry
        {
            using NameBuffer = FixedArray<char, MaxEntityNameSize>;

            NameBuffer Name;
            uint64 NameId;

            uint64 Parent;
            uint64 FirstChild;
            uint64 Next;

            UUID GetId() const;

            FORCE_INLINE bool operator==(const EntityEntry& name) const
            {
                return Name == name.Name && NameId == name.NameId;
            }

            FORCE_INLINE bool operator!=(const EntityEntry& name) const
            {
                return Name != name.Name && NameId != name.NameId;
            }

            FORCE_INLINE bool operator==(const StringView& name) const
            {
                return Name.Data() == name.Data();
            }

            FORCE_INLINE bool operator!=(const StringView& name) const
            {
                return Name.Data() != name.Data();
            }
        };

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
