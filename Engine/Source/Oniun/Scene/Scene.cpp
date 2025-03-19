#include "Oniun.pch.h"
#include "Oniun/Scene/Scene.h"

#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& title)
    : m_Title(title)
{
}

Entity Scene::Add(const StringView& name)
{
    EntityName key = {};
    key.Name.Resize(name.Length() + 1);
    Crt::Copy(key.Name.Data(), name.Data(), name.Length());
    key.Name.Data()[key.Name.Count() - 1] = '\0';

    while (m_Entities.Contains(key))
        ++key.OffsetCounter;
    auto&[entName, entEntry] = m_Entities.Add(key, EntityEntry{nullptr, nullptr, nullptr});
    return Entity(&entName, &entEntry, this);
}

Entity Scene::Find(const EntityName& name)
{
    Pair<EntityName, EntityEntry>* entity = m_Entities.TryGetPair(name);
    if (entity)
        return Entity(&entity->Key, &entity->Value, this);
    return Entity();
}

Array<Entity> Scene::Find(const StringView& name)
{
    EntityName key = {};
    key.Name.Resize(name.Length() + 1);
    Crt::Copy(key.Name.Data(), name.Data(), name.Length());
    key.Name.Data()[key.Name.Count() - 1] = '\0';

    Array<Entity> entities;
    Pair<EntityName, EntityEntry>* entity = m_Entities.TryGetPair(key);
    while (entity)
    {
        entities.Add(Entity(&entity->Key, &entity->Value, this));
        ++key.OffsetCounter;
        entity = m_Entities.TryGetPair(key);
    }
    return entities;
}

void Scene::Remove(const Entity& entity)
{
    m_Entities.Remove(*entity.m_Name);
}

bool Scene::IsAlive(const Entity& entity)
{
    return m_Entities.Contains(*entity.m_Name);
}
