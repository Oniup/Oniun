#include "Oniun/Scene/ComponentPool.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/SceneLayer.h"

uint64 ComponentPool::GetChunkSize(const Type::List& types)
{
    uint64 size = sizeof(uint64);
    for (const Type& type : types)
        size += type.Size;
    return size;
}

ComponentPool::ComponentPool(const Type::List& types)
    : m_Types(types), m_ChunkSize(GetChunkSize(types)), m_Offset(GetBlockSize())
{
}

ComponentPool::ComponentPool(ComponentPool&& registry)
    : m_Types(Memory::Move(registry.m_Types)), m_ChunkSize(registry.m_ChunkSize), m_Offset(registry.m_Offset),
      m_Entities(Memory::Move(registry.m_Entities)), m_Blocks(Memory::Move(registry.m_Blocks)),
      m_Freed(Memory::Move(registry.m_Freed))
{
}

ComponentPool::~ComponentPool()
{
    for (HeapAllocation::Data<byte>& block : m_Blocks)
    {
    }
}

uint64 ComponentPool::GetBlockSize() const
{
    return m_ChunkSize * SceneLayer::GetComponentPoolChunksPerBlockCount();
}

byte* ComponentPool::Allocate(uint64 entity)
{
    byte* data;
    if (!m_Freed.IsEmpty())
        data = m_Freed.Pop();
    else
    {
        uint64 newOffset = m_Offset + m_ChunkSize;
        if (newOffset > GetBlockSize())
        {
            newOffset = m_ChunkSize;
            m_Offset = 0;
            m_Blocks.AddLast(Memory::Move(HeapAllocation::Data<byte>()));
            m_Blocks.Last().Allocate(GetBlockSize());
        }
        data = m_Blocks.Last().Ptr() + m_Offset;
        m_Offset = newOffset;
    }
    *(uint64*)data = entity;
    m_Entities.Add(entity, data);
    return data + sizeof(uint64);
}

byte* ComponentPool::Get(uint64 entity)
{
    if (!m_Entities.Contains(entity))
        return nullptr;
    return m_Entities.At(entity);
}

byte* ComponentPool::Get(uint64 entity, const Type& type)
{
    byte* data = Get(entity);
    if (!data)
        return nullptr;
    return GetFromChunk(data, type.Id);
}

byte* ComponentPool::Get(uint64 entity, uint64 id)
{
    byte* data = Get(entity);
    if (!data)
        return nullptr;
    return GetFromChunk(data, id);
}

byte* ComponentPool::GetFromChunk(byte* chunk, uint64 id)
{
    byte* data = chunk + sizeof(uint64);
    for (const Type& type : m_Types)
    {
        if (type.Id == id)
            break;
        data = data + type.Size;
    }
    return data;
}
