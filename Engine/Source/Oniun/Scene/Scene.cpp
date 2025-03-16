#include "Oniun.pch.h"
#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Math.h"
#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& name)
    : m_Name(name)
{
}

Entity Scene::Create()
{
    uint64 id = Math::RandomUInt64();
    m_Entities.Add(id);
    return Entity(this, id);
}

void Scene::Destroy(uint64 entity)
{
    uint64 index = m_Entities.Find(entity);
    if (index == INVALID_INDEX)
        return;
    m_Entities.RemoveAt(index);
    for (auto&[id, pool] : m_Registries)
    {
        // Remove component based on entity id
    }
}

bool Scene::IsEntityAlive(uint64 entity) const
{
    uint64 index = m_Entities.Find(entity);
    return index != INVALID_INDEX;
}

byte* Scene::AllocateComponent(uint64 entity, const ComponentType& type)
{
    if (!m_ComponentsPools.Contains(type.Id))
        AddComponentRegistry(type);

    if (!m_Registries.Contains(type.Id))
        AddComponentRegistry(type.Id, type);
    return m_Registries.At(type.Id).Allocate(entity);
}

void Scene::AddComponentRegistry(const ComponentType::List& types)
{
    uint64 hash;
    if (types.Count() > 1)
        hash = Hash<ComponentType::List>{}.Get(types);
    else
        hash = types[0].Id;
    if (!m_Registries.Contains(hash))
    {
        AddComponentRegistry(hash, types);
    }
    else
        LOG(Warning,
            "Duplicate component registries added, please check all the times you use AddComponentRegistry() and remove"
            " redundant duplicates");
}

void Scene::AddComponentRegistry(const ComponentType& type)
{
    ComponentType::List asList = {type};
    AddComponentRegistry(asList);
}

void Scene::AddComponentRegistry(uint64 queryHash, const ComponentType::List& types)
{
    m_Registries.Add(queryHash, ComponentPool(types));
    ComponentPool& pool = m_Registries.At(queryHash);
    for (const ComponentType& type : types)
    {
        if (!m_ComponentsPools.Contains(type.Id))
            m_ComponentsPools.Add(type.Id, {});
        m_ComponentsPools.At(type.Id).Add(&pool);
    }
}

void Scene::AddComponentRegistry(uint64 queryHash, const ComponentType& type)
{
    ComponentType::List asList = {type};
    AddComponentRegistry(queryHash, asList);
}

byte* Scene::GetComponentData(uint64 entity, const ComponentType& type)
{
    if (!m_ComponentsPools.Contains(type.Id))
        return nullptr;
    Array<ComponentPool*>& compPools = m_ComponentsPools.At(type.Id);
    for (ComponentPool* pool : compPools)
    {
        byte* data = pool->Get(entity, type);
        if (data)
            return data;
    }
    return nullptr;
}

