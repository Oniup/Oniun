#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/Entity.h"

namespace Oniun
{
    Scene::Scene(const StringView& title)
        : m_Title(title)
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::Add(const StringView& name)
    {
        ASSERT(name.Length() + 1 < EntityEntry::MaxNameSize);
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
        while (m_Entities.Contains(id))
        {
            ++newEntry.NameId;
            id = newEntry.GetId();
        }
        // Add
        m_Entities.Add(id, newEntry);
        return Entity(id, this);
    }

    Entity Scene::Find(const EntityEntry& name)
    {
        UUID id = name.GetId();
        if(m_Entities.Contains(id))
            return Entity(id, this);
        return Entity::Invalid;
    }

    Entity Scene::Find(UUID entityId)
    {
        EntityEntry* entry = m_Entities.Try(entityId);
        if (entry)
            return Entity(entityId, this);
        return Entity::Invalid;
    }

    void Scene::RenameEntity(Entity& entity)
    {
        // TODO: ...
    }

    void Scene::Remove(const Entity& entity)
    {
        m_Entities.Remove(entity.GetId());
        for (auto&[compId, pool] : m_Pools)
            pool.Remove(entity);
    }

    bool Scene::IsAlive(const Entity& entity)
    {
        return m_Entities.Contains(entity.GetId());
    }

    byte* Scene::AddComponent(UUID entity, const ComponentType& type)
    {
        if (!m_Entities.Contains(entity))
        {
            LOG(Warning, "Entity doesn't, cannot add component");
            return nullptr;
        }
        // Get pool with desired component type
        ComponentPool* pool = m_Pools.Try(type.Id);
        if (!pool)
        {
            // Add if doesn't exist
            m_Pools.Add(type.Id, ComponentPool(type));
            pool = &m_Pools.At(type.Id);
        }
        // Allocate space for component and get uninitialized byte data
        byte* byteData = pool->Allocate(entity);
        if (!byteData)
        {
            EntityEntry& entry = m_Entities.At(entity);
            LOG(Error, "Failed to add component to entity '{} ({})'", entry.Name.Data(), entry.NameId);
        }
        return byteData;
    }

    byte* Scene::GetComponent(UUID entity, const ComponentType& type)
    {
        ComponentPool* pool = m_Pools.Try(type.Id);
        if (!pool)
            return nullptr;
        return pool->Get(entity);
    }
}
