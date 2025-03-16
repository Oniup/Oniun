#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/Function.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Core/Templates/LinkList.h"
#include "Oniun/Serialization/TypeInfo.h"

/// Blocks of memory linked together using a link list.
/// Used for packing components as tightly together without relocating them when having to resize
///
/// Memory order of a block:
/// │ Entity 1                                       │  Entity 2
/// ├──────────┬──────────────────┬──────────────────┼──────────┬───┄┄
/// │ EntityId │ Comp Type 1 Data │ Comp Type 2 Data │ EntityId │ ...
/// └──────────┴──────────────────┴──────────────────┴──────────┴───┄┄
class ComponentPool
{
public:
    struct Type
    {
        static constexpr uint64 MaxCount = 5;
        using List = FixedArray<Type, MaxCount>;

        uint64 Id;
        uint64 Size;
        Function<void(void* data)> DestructFn;

        template <typename TComponent>
        static Type Get()
        {
            Type type;
            type.Id = TypeInfo::GetFastId<TComponent>();
            type.Size = sizeof(TComponent);
            type.DestructFn.Bind([](void* data)
            {
                TComponent* component = static_cast<TComponent*>(data);
                Memory::DestructItem(component);
            });
            return type;
        }
    };

public:
    static uint64 GetChunkSize(const Type::List& types);

public:
    ComponentPool(const Type::List& types);
    ComponentPool(ComponentPool&& registry);
    ~ComponentPool();

public:
    /// Total byte size of the allocated blocks
    uint64 GetBlockSize() const;

    /// Allocates a new chunk of memory of given entity
    /// @note Will have to manually move the pointer to the start of the component data by adding sizeof(uint64)
    ///
    /// @param entity Target entity to allocate component space for
    /// @return Byte data starting from the entity id
    byte* Allocate(uint64 entity);

    /// Get the entity's component data within the pool
    ///
    /// @param entity Target entity to get components from
    /// @return Component data starting from the entity ID otherwise if entity doesn't exist, returns nullptr
    byte* Get(uint64 entity);

    /// Get the entity's specific component type from the pool
    ///
    /// @param entity Target entity to get components from
    /// @param type   Target component type to get from the pool
    /// @return Target component byte data otherwise if entity doesn't exist, returns nullptr
    byte* Get(uint64 entity, const Type& type);

    /// Get the entity's specific component type from the pool
    ///
    /// @param entity Target entity to get components from
    /// @param id     Target component id to get from the pool
    /// @return Target component byte data otherwise if entity doesn't exist, returns nullptr
    byte* Get(uint64 entity, uint64 id);

    /// Get component id from component chunk
    /// @warning Chunk data must start from the entity ID
    ///
    /// @param chunk Chunk byte data starting from the entity ID
    /// @param id    Target component ID to get
    /// @return
    byte* GetFromChunk(byte* chunk, uint64 id);

private:
    Type::List m_Types;
    uint64 m_ChunkSize;
    uint64 m_Offset;
    HashMap<uint64, byte*> m_Entities;
    LinkList<HeapAllocation::Data<byte>> m_Blocks;
    Array<byte*> m_Freed;
};

template<>
struct Hash<ComponentPool::Type::List>
{
    FORCE_INLINE constexpr uint64 Get(const ComponentPool::Type::List& types) const
    {
        uint64 ids[ComponentPool::Type::MaxCount];
        for (uint64 i = 0; i < types.Count(); ++i)
            ids[i] = types[i].Id;
        return Crt::FnvHash(ids, sizeof(uint64) * types.Count());
    }
};
