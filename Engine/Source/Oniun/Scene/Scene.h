#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/String/String.h"

class Entity;

class Scene
{
public:
    static constexpr uint64 MaxEntityNameSize = 30;
    static constexpr uint64 MaxEntityFullNameSize = MaxEntityNameSize + 32;

private:
    struct EntityName
    {
        using NameBuffer = FixedArray<char, MaxEntityNameSize>;

        NameBuffer Name;
        uint64 OffsetCounter;

        FORCE_INLINE bool operator==(const EntityName& name) const
        {
            return Name == name.Name && OffsetCounter == name.OffsetCounter;
        }

        FORCE_INLINE bool operator!=(const EntityName& name) const
        {
            return Name != name.Name && OffsetCounter != name.OffsetCounter;
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


    struct EntityEntry
    {
        EntityName* Parent;
        EntityName* FirstChild;
        EntityName* Next;
    };

    friend Entity;
    friend Hash<EntityName>;

public:
    Scene(const StringView& title = "Empty Scene");

public:
    FORCE_INLINE const String& GetTitle() const
    {
        return m_Title;
    }

    FORCE_INLINE HashMap<EntityName, EntityEntry>& GetEntityEntries()
    {
        return m_Entities;
    }

    Entity Add(const StringView& name = "Entity");
    Entity Find(const EntityName& name);
    Array<Entity> Find(const StringView& name);
    void Remove(const Entity& entity);
    bool IsAlive(const Entity& entity);

    template <typename TComponent, typename... TArgs>
    TComponent* AddComponent(EntityEntry entity, TArgs&&... args);

private:
    String m_Title;

    HashMap<EntityName, EntityEntry> m_Entities;
};

template <typename TComponent, typename ... TArgs>
TComponent* Scene::AddComponent(EntityEntry entity, TArgs&&... args)
{
    return nullptr;
}

template <>
struct Hash<Scene::EntityName>
{
    FORCE_INLINE constexpr uint64 Get(const Scene::EntityName& src) const
    {
        return Crt::FnvHash(src.Name.Data(), src.Name.Count()) + src.OffsetCounter;
    }
};
