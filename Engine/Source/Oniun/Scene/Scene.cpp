#include "Oniun/Scene/Scene.h"

#include "SceneLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/Entity.h"

namespace Oniun
{
    Scene::Scene(const StringView& title)
        : m_Title(title)
    {
    }

    Scene::Scene(Scene&& scene)
        : m_Title(Memory::Move(scene.m_Title)), m_Entities(Memory::Move(scene.m_Entities)), m_Pools(Memory::Move(scene.m_Pools))
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::Add(const StringView& name)
    {
        FORCE_ASSERT(name.Length() + 1 < EntityEntry::MaxNameSize);
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
        Entity entity(id, this);
        return entity;
    }

    Entity Scene::Find(const EntityEntry& name)
    {
        UUID id = name.GetId();
        if(m_Entities.Contains(id))
            return Entity(id, this);
        return Entity::Invalid;
    }

    Entity Scene::Find(UUID entity)
    {
        EntityEntry* entry = m_Entities.Try(entity);
        if (entry)
            return Entity(entity, this);
        return Entity::Invalid;
    }

    Pair<UUID, EntityEntry*> Scene::FindAsPair(const EntityEntry& name)
    {
        UUID id = name.GetId();
        EntityEntry* entry = m_Entities.Try(id);
        if (entry)
            return Pair(id, entry);
        return Pair<UUID, EntityEntry*>(NO_POS, nullptr);
    }

    Pair<UUID, EntityEntry*> Scene::FindAsPair(UUID entity)
    {
        EntityEntry* entry = m_Entities.Try(entity);
        if (entry)
            return Pair(entity, entry);
        return Pair<UUID, EntityEntry*>(NO_POS, nullptr);
    }

    void Scene::RenameEntity(Entity& entity)
    {
        // TODO: ...
    }

    void Scene::Remove(const Entity& entity)
    {
        // Remove all children entities
        Entity child = entity.GetFirstChild();
        while (child != Entity::Invalid)
        {
            Entity next = child.GetNextSibling();
            Remove(child);
            child = next;
        }

        m_Entities.Remove(entity.GetId());
        for (auto& [compId, pool] : m_Pools)
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
            m_Pools.Add(type.Id, Memory::Move(ComponentPool(type)));
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
