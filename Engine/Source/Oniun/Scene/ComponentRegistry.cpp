#include "Oniun/Scene/ComponentRegistry.h"

#include "Oniun/Core/Logger.h"

uint64 ComponentRegistry::CreateInfo::GetQueryHash() const
{
    uint64 queryHash = 0;
    for (uint64 i = 0; i < Count; ++i)
        queryHash += Ids[i];
    return queryHash;
}

ComponentRegistry::ComponentRegistry()
    : m_LastEntity(INVALID_INDEX), m_Offset(0), m_ComponentBlockSize(sizeof(uint64))
{
}

ComponentRegistry::ComponentRegistry(ComponentRegistry&& registry)
    : m_LastEntity(registry.m_LastEntity), m_ComponentTypes(Memory::Move(registry.m_ComponentTypes)), m_Offset(0),
      m_ComponentBlockSize(0), m_Data(Memory::Move(registry.m_Data)), m_Freed(Memory::Move(registry.m_Freed))
{
}

ComponentRegistry::~ComponentRegistry()
{
    if (m_Data.Ptr())
    {
        for (const auto&[entityId, offset] : m_EntitiesCompIndex)
        {
            byte* compData = m_Data.Ptr() + offset + sizeof(uint64);
            for (const Type& type : m_ComponentTypes)
            {
                type.DestructFn(compData);
                compData = compData + type.Size;
            }
        }
        m_Data.Free();
        m_Offset = 0;
    }
}

bool ComponentRegistry::RegisterComponentTypes(const CreateInfo& createInfo)
{
    if (createInfo.Count >= MaxTypeCount)
    {
        LOG(Error, "Cannot add more than {} components to a component registry.", MaxTypeCount);
        return false;
    }
    for (uint64 i = 0; i < createInfo.Count; ++i)
    {
        m_ComponentBlockSize += createInfo.Sizes[i];
        m_ComponentTypes.Add({
            .Id = createInfo.Ids[i],
            .Size = createInfo.Sizes[i],
            .DestructFn = createInfo.DestructFns[i],
        });
    }
    return true;
}

byte* ComponentRegistry::AllocateComponents(uint64 entityId, uint64 compId)
{
    byte* compData;
    if (m_Freed.Count() > 0)
    {
        uint64 offset = m_Freed.Pop();
        compData = m_Data.Ptr() + offset;
        m_EntitiesCompIndex.Add(entityId, offset);
        *(uint64*)(compData) = entityId;
    }
    else if (m_EntitiesCompIndex.Contains(entityId))
    {
        compData = m_Data.Ptr() + m_EntitiesCompIndex.Get(entityId);
    }
    else
    {
        uint64 newSize = m_Offset + m_ComponentBlockSize;
        if (m_Data.Capacity() < newSize)
            Resize(newSize);

        compData = m_Data.Ptr() + m_Offset;
        m_EntitiesCompIndex.Add(entityId, m_Offset);
        m_Offset = newSize;
        *(uint64*)(compData) = entityId;
    }
    return GetComponentFromAllocBlock(compId, compData);
}

bool ComponentRegistry::SupportsComponents(const uint64* compIds, uint64 count) const
{
    if (count != m_ComponentTypes.Count())
        return false;

    for (uint64 i = 0; i < count; ++i)
    {
        for (uint64 j = 0; j < count; ++j)
        {
            if (compIds[i] != m_ComponentTypes[i].Id)
                return false;
        }
    }
    return true;
}

bool ComponentRegistry::SupportsComponents(const FixedArray<Type, MaxTypeCount>& types) const
{
    if (types.Count() != m_ComponentTypes.Count())
        return false;

    for (uint64 i = 0; i < types.Count(); ++i)
    {
        for (uint64 j = 0; j < types.Count(); ++j)
        {
            if (types[i].Id != m_ComponentTypes[i].Id)
                return false;
        }
    }
    return true;
}

bool ComponentRegistry::SupportsAllComponents(const uint64* compIds, uint64 count) const
{
    if (count != m_ComponentTypes.Count())
        return false;

    uint64 found = 0;
    for (uint64 i = 0; i < count; ++i)
    {
        for (uint64 j = 0; j < count; ++j)
        {
            if (compIds[i] != m_ComponentTypes[i].Id)
            {
                ++found;
                break;
            }
        }
    }
    return found == count;
}

bool ComponentRegistry::SupportsAllComponents(const FixedArray<Type, MaxTypeCount>& types) const
{
    if (types.Count() != m_ComponentTypes.Count())
        return false;

    uint64 found = 0;
    for (uint64 i = 0; i < types.Count(); ++i)
    {
        for (uint64 j = 0; j < types.Count(); ++j)
        {
            if (types[i].Id == m_ComponentTypes[i].Id)
            {
                ++found;
                break;
            }
        }
    }
    return found == types.Count();
}

byte* ComponentRegistry::GetComponents(uint64 entityId)
{
    uint64* offset = m_EntitiesCompIndex.TryGet(entityId);
    if (offset)
        return m_Data.Ptr() + *offset + sizeof(uint64);
    return nullptr;
}

byte* ComponentRegistry::GetComponent(uint64 entityId, uint64 compId)
{
    uint64* offset = m_EntitiesCompIndex.TryGet(entityId);
    if (offset)
        return GetComponentFromAllocBlock(compId, m_Data.Ptr() + *offset);
    return nullptr;
}

void ComponentRegistry::Remove(uint64 entityId)
{
    uint64* offset = m_EntitiesCompIndex.TryGet(entityId);
    if (offset)
    {
        byte* compData = m_Data.Ptr();
        *(uint64*)compData = INVALID_INDEX;
        m_EntitiesCompIndex.Remove(entityId);

        compData = compData + sizeof(uint64);
        for (const Type& type : GetComponentTypes())
        {
            type.DestructFn(compData);
            compData = compData + type.Size;
        }
    }
}

void ComponentRegistry::RemoveWithoutDestructor(uint64 entityId)
{
    uint64* offset = m_EntitiesCompIndex.TryGet(entityId);
    if (offset)
    {
        byte* compData = m_Data.Ptr();
        *(uint64*)compData = INVALID_INDEX;
        m_EntitiesCompIndex.Remove(entityId);
    }
}

byte* ComponentRegistry::GetComponentFromAllocBlock(uint64 compId, byte* fullCompData)
{
    byte* compData = fullCompData + sizeof(uint64);
    for (const Type& type : m_ComponentTypes)
    {
        if (type.Id == compId)
            break;
        compData = compData + type.Size;
    }
    return compData;
}

void ComponentRegistry::Resize(uint64 newSize)
{
    m_Data.Relocate(Memory::CalcCapacityGrow(newSize, m_Data.Capacity()), m_Offset, newSize);
    for (uint64 offset = m_Offset; offset < m_Data.Capacity(); offset += m_ComponentBlockSize)
        *(uint64*)m_Data.Ptr() = INVALID_INDEX;
}
