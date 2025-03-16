#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Scene/ComponentPool.h"

class Entity;

class Scene
{
public:
    using ComponentType = ComponentPool::Type;

public:
    Scene(const StringView& name = "Empty Scene");

public:
    FORCE_INLINE const String& GetName() const
    {
        return m_Name;
    }

    Entity Create();
    void Destroy(uint64 entity);
    bool IsEntityAlive(uint64 entity) const;

    template <typename TComponent, typename... TArgs>
    void AddComponent(uint64 entity, TArgs&&... args)
    {
        ComponentType type = ComponentType::Get<TComponent>();
        byte* data = AllocateComponent(entity, type);
        if (data)
        {
            TComponent* component = (TComponent*)data;
            Memory::ConstructItem(component, Memory::Forward<TArgs>(args)...);
        }
    }

    template <typename TComponent>
    TComponent* GetComponent(uint64 entity)
    {
        byte* data = GetComponentData(entity, ComponentType::Get<TComponent>());
        return (TComponent*)data;
    }

    template <typename... TComponents>
    void AddComponentRegistry()
    {
        ComponentType::List types = {ComponentType::Get<TComponents>()...};
        AddComponentRegistry(types);
    }

private:
    byte* AllocateComponent(uint64 entity, const ComponentType& type);
    void AddComponentRegistry(const ComponentType::List& types);
    void AddComponentRegistry(const ComponentType& type);
    void AddComponentRegistry(uint64 queryHash, const ComponentType::List& types);
    void AddComponentRegistry(uint64 queryHash, const ComponentType& type);

    byte* GetComponentData(uint64 entity, const ComponentType& type);

private:
    String m_Name;
    Array<uint64> m_Entities;
    HashMap<uint64, Array<ComponentPool*>> m_ComponentsPools;
    HashMap<uint64, ComponentPool> m_Registries;
};
