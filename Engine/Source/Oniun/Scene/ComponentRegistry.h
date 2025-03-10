#pragma once

#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Serialization/TypeInfo.h"

/// Registry for storing components packed in an array
///
/// Memory order:
/// │ Entity 1                                       │  Entity 2
/// ├──────────┬──────────────────┬──────────────────┼──────────┬───┄┄
/// │ EntityId │ Comp Type 1 Data │ Comp Type 2 Data │ EntityId │ ...
/// └──────────┴──────────────────┴──────────────────┴──────────┴───┄┄
class ComponentRegistry
{
public:
    static constexpr uint64 MaxTypeCount = 5;
    typedef void (*PfnDestructComponent)(byte* compData);

public:
    struct Type
    {
        uint64 Id;
        uint64 Size;
        PfnDestructComponent DestructFn;

        template <typename TComponent>
        static Type Get()
        {
            return Type {
                .Id = TypeInfo::GetFastId<TComponent>(),
                .Size = sizeof(TComponent),
                .DestructFn = {
                    [](byte* compData)
                    {
                        TComponent* component = (TComponent*)compData;
                        Memory::DestructItem(component);
                    }
                },
            };
        }
    };

    struct CreateInfo
    {
        uint64 Count;
        uint64 Ids[MaxTypeCount];
        uint64 Sizes[MaxTypeCount];
        PfnDestructComponent DestructFns[MaxTypeCount];

        template <typename... TComponents>
        static CreateInfo Get()
        {
            return CreateInfo{
                .Count = sizeof...(TComponents),
                .Ids = {TypeInfo::GetFastId<TComponents>()...},
                .Sizes = {sizeof(TComponents)...},
                .DestructFns = {
                    [](byte* compData)
                    {
                        TComponents* component = (TComponents*)compData;
                        Memory::DestructItem(component);
                    }...
                },
            };
        }

        uint64 GetQueryHash() const;
    };

public:
    ComponentRegistry();
    ComponentRegistry(ComponentRegistry&& registry);
    ~ComponentRegistry();

public:
    FORCE_INLINE uint64 GetComponentBlockSize() const
    {
        return m_ComponentBlockSize;
    }

    FORCE_INLINE const FixedArray<Type, MaxTypeCount>& GetComponentTypes() const
    {
        return m_ComponentTypes;
    }

    FORCE_INLINE uint64 GetCount() const
    {
        return m_Offset / m_ComponentBlockSize;
    }

public:
    bool RegisterComponentTypes(const CreateInfo& createInfo);
    byte* AllocateComponents(uint64 entityId, uint64 compId);

    bool SupportsComponents(const uint64* compIds, uint64 count) const;
    bool SupportsComponents(const FixedArray<Type, MaxTypeCount>& types) const;
    bool SupportsAllComponents(const uint64* compIds, uint64 count) const;
    bool SupportsAllComponents(const FixedArray<Type, MaxTypeCount>& types) const;

    byte* GetComponents(uint64 entityId);
    byte* GetComponent(uint64 entityId, uint64 compId);

    void Remove(uint64 entityId);
    void RemoveWithoutDestructor(uint64 entityId);

private:
    byte* GetComponentFromAllocBlock(uint64 compId, byte* fullCompData);
    void Resize(uint64 newSize);

private:
    uint64 m_LastEntity;
    /// Key: Entity ID, Value: Position
    HashMap<uint64, uint64> m_EntitiesCompIndex;
    FixedArray<Type, MaxTypeCount> m_ComponentTypes;
    uint64 m_Offset;
    uint64 m_ComponentBlockSize;
    HeapAllocation::Data<byte> m_Data;
    Array<uint64> m_Freed;
};
