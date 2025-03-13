#include "Oniun/Scene/ComponentRegistry.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/SceneLayer.h"

uint64 ComponentRegistry::CreateInfo::GetQueryHash() const
{
    uint64 queryHash = 0;
    for (uint64 i = 0; i < Count; ++i)
        queryHash += Ids[i];
    return queryHash;
}

ComponentRegistry::ComponentRegistry()
    : m_Offset(0), m_ComponentBlockSize(sizeof(uint64))
{
}

ComponentRegistry::ComponentRegistry(ComponentRegistry&& registry)
    : m_ComponentTypes(Memory::Move(registry.m_ComponentTypes)), m_Offset(0), m_ComponentBlockSize(0),
      m_Data(Memory::Move(registry.m_Data)), m_Freed(Memory::Move(registry.m_Freed))
{
}

ComponentRegistry::~ComponentRegistry()
{
    for (auto&[entityId, compDataPtr] : m_EntitiesCompIndex)
    {
        byte* compData = compDataPtr + sizeof(uint64);
        for (const Type& type : m_ComponentTypes)
        {
            type.DestructFn(compData);
            compData = compData + type.Size;
        }
    }
    m_Data.Free();
    m_Offset = 0;
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
        compData = m_Freed.Pop();
        m_EntitiesCompIndex.Add(entityId, compData);
        *(uint64*)(compData) = entityId;
    }
    else if (m_EntitiesCompIndex.Contains(entityId))
    {
        compData = m_EntitiesCompIndex.Get(entityId);
    }
    else
    {
        if (m_Offset < m_ComponentBlockSize * SceneLayer::GetComponentRegistryChunksPerBlockCount())
        {
            m_Data.AddLast(Memory::Move(BlockData()));
            m_Data.Last().Allocate(m_ComponentBlockSize * SceneLayer::GetComponentRegistryChunksPerBlockCount());
            m_Offset = 0;
        }

        uint64 newOffset = m_Offset + m_ComponentBlockSize;
        compData = m_Data.Last().Ptr() + m_Offset;
        m_EntitiesCompIndex.Add(entityId, compData);
        m_Offset = newOffset;
        *(uint64*)(compData) = entityId;
    }
    return GetComponentFromAllocBlock(compId, compData);
}

bool ComponentRegistry::SupportsComponents(const uint64* compIds, uint64 count) const
{
    uint64 found = 0;
    for (uint64 i = 0; i < count; ++i)
    {
        for (uint64 j = 0; j < m_ComponentTypes.Count(); ++j)
        {
            if (compIds[i] == m_ComponentTypes[j].Id)
            {
                ++found;
                break;
            }
        }
    }
    return found == count;
}

bool ComponentRegistry::SupportsComponents(const FixedArray<Type, MaxTypeCount>& types) const
{
    uint64 found = 0;
    for (uint64 i = 0; i < types.Count(); ++i)
    {
        for (uint64 j = 0; j < m_ComponentTypes.Count(); ++j)
        {
            if (types[i].Id == m_ComponentTypes[j].Id)
            {
                ++found;
                break;
            }
        }
    }
    return found == types.Count();
}

bool ComponentRegistry::SupportsAllComponents(const uint64* compIds, uint64 count) const
{
    if (count != m_ComponentTypes.Count())
        return false;
    return SupportsComponents(compIds, count);
}

bool ComponentRegistry::SupportsAllComponents(const FixedArray<Type, MaxTypeCount>& types) const
{
    if (types.Count() != m_ComponentTypes.Count())
        return false;
    return SupportsComponents(types);
}

byte* ComponentRegistry::GetComponents(uint64 entityId)
{
    byte** compEntry = m_EntitiesCompIndex.TryGet(entityId);
    if (compEntry)
        return *compEntry + sizeof(uint64);
    return nullptr;
}

byte* ComponentRegistry::GetComponent(uint64 entityId, uint64 compId)
{
    byte** compEntry = m_EntitiesCompIndex.TryGet(entityId);
    if (compEntry)
        return GetComponentFromAllocBlock(compId, *compEntry);
    return nullptr;
}

void ComponentRegistry::Remove(uint64 entityId)
{
    byte** compEntry = m_EntitiesCompIndex.TryGet(entityId);
    if (compEntry)
    {
        byte* compData = *compEntry;
        *(uint64*)compData = INVALID_INDEX;

        compData = compData + sizeof(uint64);
        for (const Type& type : GetComponentTypes())
        {
            type.DestructFn(compData);
            compData = compData + type.Size;
        }

        m_Freed.Add(*compEntry);
        m_EntitiesCompIndex.Remove(entityId);
    }
}

void ComponentRegistry::RemoveWithoutDestructor(uint64 entityId)
{
    byte** compEntry = m_EntitiesCompIndex.TryGet(entityId);
    if (compEntry)
    {
        byte* compData = *compEntry;
        *(uint64*)compData = INVALID_INDEX;

        m_Freed.Add(*compEntry);
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
