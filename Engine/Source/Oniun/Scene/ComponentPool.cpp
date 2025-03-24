#include "Oniun/Scene/ComponentPool.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Scene/SceneLayer.h"

namespace Oniun
{
    ComponentPool::ComponentPool(const ComponentType& type)
        : m_AllocationCount(0), m_Type(type),
          m_BlockSize(Engine::GetLayer<SceneLayer>()->GetComponentPoolChunksPerBlockCount() * type.Size), m_Offset(m_BlockSize)
    {
    }

    ComponentPool::ComponentPool(ComponentPool&& pool)
        : m_AllocationCount(pool.m_AllocationCount), m_Type(Memory::Move(pool.m_Type)),
          m_BlockSize(pool.m_BlockSize), m_Offset(pool.m_Offset), m_Data(Memory::Move(pool.m_Data)),
          m_EntityToComp(Memory::Move(pool.m_EntityToComp)), m_FreedLocations(Memory::Move(pool.m_FreedLocations))
    {
    }

    ComponentPool::~ComponentPool()
    {
        Clear();
    }

    byte* ComponentPool::Allocate(UUID entity)
    {
        if (m_EntityToComp.Contains(entity))
        {
            LOG(Warning, "Cannot add more than two of the same components to the same entity");
            return nullptr;
        }

        byte* byteData;
        if (m_FreedLocations.Count() > 0)
            byteData = m_FreedLocations.Pop();
        else
        {
            if (m_Offset >= m_BlockSize)
            {
                m_Data.AddLast(HeapAllocation::Data<byte>());
                m_Data.Last().Allocate(m_BlockSize);
                m_Offset = 0;
            }
            byteData = m_Data.Last().Ptr() + m_Offset;
            m_Offset += m_Type.Size;
        }
        m_EntityToComp.Add(entity, byteData);
        ++m_AllocationCount;
        return byteData;
    }

    byte* ComponentPool::Get(UUID entity)
    {
        byte** byteData = m_EntityToComp.Try(entity);
        if (byteData)
            return *byteData;
        return nullptr;
    }

    void ComponentPool::Remove(UUID entity)
    {
        if (!m_EntityToComp.Contains(entity))
            return;

        byte* byteData = m_EntityToComp.At(entity);
        m_Type.DestructFn(byteData);
        if (m_EntityToComp.Remove(entity))
        {
            --m_AllocationCount;
            if (m_Offset > 0)
            {
                // Check if the last slot added
                byte* current = m_Data.Last().Ptr() + m_Offset;
                current = current - m_Type.Size;
                if (current == byteData)
                    m_Offset -= m_Type.Size;
                return;
            }
            m_FreedLocations.Add(byteData);
        }
    }

    void ComponentPool::Clear()
    {
        if (m_AllocationCount > 0)
        {
            for (auto&[entity, byteData] : m_EntityToComp)
                m_Type.DestructFn(byteData);
            m_AllocationCount = 0;
        }
    }
}
