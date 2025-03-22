#pragma once

#include "Oniun/Core/UUID.h"
#include "Oniun/Core/Templates/LinkList.h"
#include "Oniun/Serialization/TypeInfo.h"

namespace Oniun
{
    struct ComponentType
    {
        uint64 Id;
        uint64 Size;
        Function<void(byte* compBytes)> DestructFn;

        template <typename TComponent>
        static constexpr ComponentType Get()
        {
            return ComponentType{
                .Id = TypeInfo::GetFastId<TComponent>(),
                .Size = sizeof(TComponent),
                .DestructFn = [](byte* compBytes)
                {
                    TComponent* comp = reinterpret_cast<TComponent*>(compBytes);
                    Memory::DestructItem(comp);
                },
            };
        }
    };

    class ComponentPool
    {
    public:
        ComponentPool(const ComponentType& type);
        ComponentPool(ComponentPool&& pool);
        ~ComponentPool();

    public:
        FORCE_INLINE uint64 AllocationCount() const
        {
            return m_AllocationCount;
        }

        FORCE_INLINE uint64 GetBlockSize() const
        {
            return m_BlockSize;
        }

    public:
        byte* Allocate(UUID entity);
        byte* Get(UUID entity);
        void Remove(UUID entity);

    private:
        uint64 m_AllocationCount;
        ComponentType m_Type;
        uint64 m_BlockSize;
        uint64 m_Offset;

        LinkList<HeapAllocation::Data<byte>> m_Data;
        HashMap<uint64, byte*> m_EntityToComp;
        Array<byte*> m_FreedLocations;
    };
}
