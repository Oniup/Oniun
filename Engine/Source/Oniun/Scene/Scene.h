#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Scene/ComponentRegistry.h"
#include "Oniun/Scene/Query.h"
#include "Oniun/Serialization/TypeInfo.h"

class Entity;

class Scene
{
public:
    using ComponentType = ComponentRegistry::Type;

    struct EntityEntry
    {
        uint64 Id;
        EntityEntry* Parent;
        Array<EntityEntry*> Children;

        FORCE_INLINE bool operator==(uint64 entityId) const
        {
            return entityId == Id;
        }
    };

public:
    Scene(const StringView& title = "Empty");
    ~Scene();

public:
    FORCE_INLINE const String& GetTitle() const
    {
        return m_Title;
    }

    Entity Create();
    void Destroy(uint64 entityId);
    bool EntityIsAlive(uint64 entityId);

    template <typename... TComponents>
    bool AddComponentRegistry()
    {
        ComponentRegistry::CreateInfo info = ComponentRegistry::CreateInfo::Get<TComponents...>();
        uint64 queryHash = info.GetQueryHash();
        if (m_Registries.Contains(queryHash))
            return true;
        return AddComponentRegistry(info);
    }

    template <typename TComponent, typename... TArgs>
    void AddComponent(uint64 entityId, TArgs&&... args)
    {
        const ComponentType compType = ComponentType::Get<TComponent>();
        byte* compData = AddComponentToRegistry(entityId, compType);
        if (compData)
        {
            TComponent* component = (TComponent*)compData;
            Memory::ConstructItemArgs(component, args...);
        }
    }


    template <typename... TComponents>
    Array<ComponentRegistry*> GetComponentRegistries()
    {
        constexpr uint64 count = sizeof...(TComponents);
        static_assert(count > 0);

        uint64 ids[] = {TypeInfo::GetFastId<TComponents>()...};
        return GetComponentRegistries(ids, count);
    }

    template <typename... TComponents>
    ComponentQuery Query()
    {
        constexpr uint64 count = sizeof...(TComponents);
        static_assert(count > 0);

        Array<ComponentRegistry*> registries = GetComponentRegistries<TComponents...>();
        uint64 ids[] = {TypeInfo::GetFastId<TComponents>()...};

        return ComponentQuery(ids, Memory::Move(registries));
    }

    template <typename TComponent>
    TComponent* GetComponent(uint64 entity)
    {
        uint64 id = TypeInfo::GetFastId<TComponent>();

        Array<ComponentRegistry*>* compRegistries = m_ComponentsRegistries.TryGet(id);
        if (compRegistries)
        {
            for (ComponentRegistry* registry : *compRegistries)
            {
                byte* compData = registry->GetComponent(entity, id);
                if (compData)
                    return (TComponent*)compData;
            }
        }
        return nullptr;
    }

private:
    bool AddComponentRegistry(const ComponentRegistry::CreateInfo& info);

    Array<ComponentRegistry*> GetComponentRegistries(const uint64* ids, uint64 count);
    byte* AddComponentToRegistry(uint64 entityId, const ComponentType& compType);

    bool GetPossibleMoveComponents(const ComponentRegistry* registry, uint64 entityId,
                                   FixedArray<ComponentRegistry*, ComponentRegistry::MaxTypeCount>& result);
    void MoveComponentsToRegistry(uint64 entityId, ComponentRegistry* from, ComponentRegistry* to);

private:
    String m_Title;
    Array<EntityEntry> m_Entities;
    HashMap<uint64, Array<ComponentRegistry*>> m_ComponentsRegistries;
    HashMap<uint64, ComponentRegistry> m_Registries;
};
