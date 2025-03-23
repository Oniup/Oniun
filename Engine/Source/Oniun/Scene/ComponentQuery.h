#pragma once

#include "Oniun/Scene/ComponentPool.h"
#include "Oniun/Scene/Entity.h"
#include "Oniun/Scene/EntityEntry.h"
#include "Oniun/Scene/Scene.h"

namespace Oniun
{
    template <typename... TComponents>
    class ComponentQuery
    {
    private:
        using EntityIterator = HashMap<UUID, EntityEntry>::Iterator;

    public:
        static constexpr uint64 QueryCount = sizeof...(TComponents);

        ComponentQuery(Scene& scene)
            : m_Iter(scene.GetEntityEntries().Begin()), m_End(scene.GetEntityEntries().End()), m_Scene(&scene)
        {
            constexpr uint64 compIds[QueryCount] = {TypeInfo::GetFastId<TComponents>()...};
            HashMap<uint64, ComponentPool>& pools = m_Scene->GetComponentPools();
            for (uint64 i = 0; i < QueryCount; ++i)
            {
                ComponentPool* pool = pools.Try(compIds[i]);
                if (!pool)
                {
                    m_Iter = m_End;
                    return;
                }
                m_Pools[i] = pool;
            }

            for (uint64 i = 0; i < QueryCount; ++i)
            {
                m_ComponentData[i] = m_Pools[i]->Get(m_Iter->Key);
                if (!m_ComponentData[i])
                {
                    Next();
                    break;
                }
            }
        }

    public:
        FORCE_INLINE ComponentQuery& operator++()
        {
            Next();
            return *this;
        }

        FORCE_INLINE Entity GetCurrentEntity() const
        {
            return Entity(m_Iter->Key, m_Scene);
        }

        FORCE_INLINE StringView GetCurrentEntityName() const
        {
            return m_Iter->Value.Name.Data();
        }

        FORCE_INLINE String GetCurrentEntityFullName() const
        {
            return Format("{}{}", m_Iter->Value.Name.Data(), m_Iter->Value.NameId);
        }

    public:
        template <typename TComponent>
        TComponent* Get()
        {
            constexpr uint64 id = TypeInfo::GetFastId<TComponent>();
            for (uint64 i = 0; i < QueryCount; ++i)
            {
                if (m_Pools[i]->GetSupportedType().Id == id)
                    return (TComponent*)m_ComponentData[i];
            }
            return nullptr;
        }

        bool IsEnd()
        {
            return m_Iter == m_End;
        }

        bool Next()
        {
            bool supportsAll;
            do
            {
                ++m_Iter;
                supportsAll = true;
                for (uint64 i = 0; i < QueryCount; ++i)
                {
                    m_ComponentData[i] = m_Pools[i]->Get(m_Iter->Key);
                    if (!m_ComponentData[i])
                    {
                        supportsAll = false;
                        break;
                    }
                }
            } while (!supportsAll && m_Iter != m_End);
            return m_Iter != m_End;
        }

    private:
        EntityIterator m_Iter;
        EntityIterator m_End;
        ComponentPool* m_Pools[QueryCount];
        byte* m_ComponentData[QueryCount];
        Scene* m_Scene;
    };
}
