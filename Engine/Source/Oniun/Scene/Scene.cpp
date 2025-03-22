#include "Oniun.pch.h"
#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& title)
    : m_Title(title)
{
}

Scene::~Scene()
{
}

Entity Scene::Add(const StringView& name)
{
    ASSERT(name.Length() + 1 < MaxEntityNameSize);
    EntityEntry newEntry = {
        .NameId = 0,
        .Parent = NO_POS,
        .FirstChild = NO_POS,
        .Next = NO_POS,
    };
    // Copy the name into buffer
    newEntry.Name.Resize(name.Length() + 1);
    Crt::Copy(newEntry.Name.Data(), name.Data(), name.Length());
    newEntry.Name.Data()[newEntry.Name.Count() - 1] = '\0';

    // Search for existing
    UUID id = newEntry.GetId();
    while (m_Entities.contains(id))
    {
        ++newEntry.NameId;
        id = newEntry.GetId();
    }
    // Add
    m_Entities.insert({id, newEntry});
    return Entity(id, this);
}

Entity Scene::Find(const EntityEntry& name)
{
    UUID id = name.GetId();
    if(m_Entities.contains(id))
        return Entity(id, this);
    return Entity::Invalid;
}

Entity Scene::Find(UUID entityId)
{
    if (m_Entities.contains(entityId))
        return Entity(entityId, this);
    return Entity::Invalid;
}

void Scene::Remove(const Entity& entity)
{
    m_Entities.erase(entity.m_Id);
}

bool Scene::IsAlive(const Entity& entity)
{
    return m_Entities.contains(entity.m_Id);
}

byte* Scene::AddComponent(UUID entity, const ComponentType& type)
{
    if (!m_Entities.contains(entity))
    {
        LOG(Warning, "Entity doesn't, cannot add component");
        return nullptr;
    }

    // Get pool with desired component type
    if (m_Pools.contains(type.Id))
    {
        // Add if doesn't exist
        m_Pools.insert({type.Id, ComponentPool(type)});
    }
    ComponentPool& pool = m_Pools.at(type.Id);
    // Allocate space for component and get uninitialized byte data
    byte* byteData = pool.Allocate(entity);
    if (!byteData)
    {
        EntityEntry& entry = m_Entities.at(entity);
        LOG(Error, "Failed to add component to entity '{} ({})'", entry.Name.Data(), entry.NameId);
    }
    return byteData;
}

byte* Scene::GetComponent(UUID entity, const ComponentType& type)
{
    if (m_Pools.contains(entity))
        return m_Pools.at(entity).Get(entity);
    return nullptr;
}

UUID Scene::EntityEntry::GetId() const
{
    return Hash_Internal::FnvHash(Name.Data(), Name.Count()) + Hash<uint64>{}.Get(NameId);
}
