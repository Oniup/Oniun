#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Serialization/TypeInfo.h"

class Entity;

class Scene
{
public:
    struct EntityEntry
    {
        uint64 Id;
        EntityEntry* Parent;
        Array<EntityEntry*> Children;
    };

private:
    // Memory layout of Registry: [EntityId, ComponentData] ...
    // ComponentData is invalid if EntityId == INVALID_INDEX
    struct ComponentRegistry
    {
        static constexpr uint64 MaxComponentCount = 5;
        typedef void (*PfnDestructComponent)(byte* compData);

        uint64 ComponentSize;
        uint64 ComponentCount;
        uint64 ComponentIds[MaxComponentCount];
        PfnDestructComponent ComponentDestructFn;
        uint64 RegisteredCount;
        HeapAllocation::Data<byte> Registry;
        Array<byte*> Freed;
    };

public:
    Scene(const StringView& title = "Empty");
    ~Scene();

public:
    FORCE_INLINE const String& GetTitle() const
    {
        return m_Title;
    }

    template<typename... TComponents>
    void AddMultiComponentRegistry()
    {
        uint64 count = sizeof...(TComponents);
        uint64 ids[] = {TypeInfo::GetFastId<TComponents>()...};
        uint64 queryHash = 0;

        ComponentRegistry registry {};
        registry.ComponentSize = (sizeof(TComponents) + ...);
        registry.ComponentCount = count;

        for (uint64 i = 0; i < count; ++i)
        {
            registry.ComponentIds[i] = ids[i];
            queryHash += ids[i];
        }

        registry.ComponentDestructFn = [](byte* compData)
        {
            uint64 offset = 0;
            auto destructItem = [&offset](auto* component)
            {
                Memory::DestructItem(component);
                offset += sizeof(component);
            };
            (destructItem((TComponents*)(compData + offset)), ...);
        };

        m_Registries.Add(queryHash, Memory::Move(registry));
    }

    Entity Create();

    template<typename... TComponents>
    void Add(uint64 entityId, TComponents&&...)
    {
        constexpr uint64 count = sizeof...(TComponents);
        static_assert(count > ComponentRegistry::MaxComponentCount, "Too many components in single registry");
    }

    void Clear();

private:
    String m_Title;
    Array<EntityEntry> m_Entities;
    HashMap<uint64, ComponentRegistry> m_Registries;
};
