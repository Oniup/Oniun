#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Math/Math.h"
#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& title)
    : m_Title(title)
{
}

Scene::~Scene()
{
    Clear();
}

Entity Scene::Create()
{
    m_Entities.Add(EntityEntry(Math::RandomUInt64(), nullptr));
    return Entity(this, m_Entities.Back().Id);
}

void Scene::Clear()
{
    m_Entities.Clear();
    for (auto&[compType, registry] : m_Registries)
    {
        for (uint64 i = 0; i < registry.RegisteredCount; ++i)
        {
            byte* entry = registry.Registry.Ptr() + (sizeof(uint64) + registry.ComponentSize) * i;
            uint64* entityId = (uint64*)entry;
            if (*entityId != INVALID_INDEX)
            {
                byte* data = entry + sizeof(uint64);
                registry.ComponentDestructFn(data);
            }
        }
    }
    m_Registries.Clear();
}
