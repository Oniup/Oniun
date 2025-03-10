#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Math.h"
#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& title)
    : m_Title(title)
{
}

Scene::~Scene()
{
}

Entity Scene::Create()
{
    m_Entities.Add(EntityEntry(Math::RandomUInt64(), nullptr));
    return Entity(this, m_Entities.Back().Id);
}

bool Scene::EntityIsAlive(uint64 entityId)
{
    uint64 index = m_Entities.Find(entityId);
    return index != INVALID_INDEX;
}

bool Scene::AddComponentRegistry(const ComponentRegistry::CreateInfo& info)
{
    ComponentRegistry& registry = m_Registries.Add(info.GetQueryHash());
    if (!registry.RegisterComponentTypes(info))
        return false;
    for (const ComponentType type : registry.GetComponentTypes())
    {
        Array<ComponentRegistry*>& compRegistriesList = m_ComponentsRegistries[type.Id];
        compRegistriesList.Add(&registry);
    }
    return true;
}

Array<ComponentRegistry*> Scene::GetComponentRegistries(const uint64* ids, uint64 count)
{
    Array<ComponentRegistry*> targetRegistries;

    // Check for existing registries
    Array<ComponentRegistry*>* compRegistries = m_ComponentsRegistries.TryGet(ids[0]);
    if (!compRegistries)
        return targetRegistries;

    // Get registries with both components and return
    targetRegistries.Reserve(compRegistries->Capacity());
    for (ComponentRegistry* registry : *compRegistries)
    {
        if (registry->SupportsComponents(ids, count))
            targetRegistries.Add(registry);
    }
    return targetRegistries;
}

byte* Scene::AddComponentToRegistry(uint64 entityId, const ComponentType& compType)
{
    if (m_ComponentsRegistries.Contains(compType.Id))
    {
        // Iterate over all component types in registry and check if they exist in other registries, if all do, then move
        // to current registry and add compType and return its byte data
        Array<ComponentRegistry*>& registries = m_ComponentsRegistries.Get(compType.Id);
        if (registries.Count() > 1)
        {
            for (ComponentRegistry* registry : registries)
            {
                FixedArray<ComponentRegistry*, ComponentRegistry::MaxTypeCount> compsToMove;
                if (!GetPossibleMoveComponents(registry, entityId, compsToMove))
                    continue;

                for (ComponentRegistry* moveFromRegistry : compsToMove)
                    MoveComponentsToRegistry(entityId, moveFromRegistry, registry);

                return registry->AllocateComponents(entityId, compType.Id);
            }
        }
    }

    if (!m_Registries.Contains(compType.Id))
    {
        ComponentRegistry::CreateInfo info;
        info.Count = 1,
        info.Ids[0] = compType.Id;
        info.Sizes[0] = compType.Size;
        info.DestructFns[0] = compType.DestructFn;
        if (!AddComponentRegistry(info))
            return nullptr;
    }
    ComponentRegistry& singleCompTypeRegistry = m_Registries.Get(compType.Id);
    return singleCompTypeRegistry.AllocateComponents(entityId, compType.Id);
}

bool Scene::GetPossibleMoveComponents(const ComponentRegistry* registry, uint64 entityId,
                                      FixedArray<ComponentRegistry*, ComponentRegistry::MaxTypeCount>& result)
{
    const auto& types = registry->GetComponentTypes();
    if (types.Count() == 1)
        return false;
    for (const ComponentType& type : types)
    {
        Array<ComponentRegistry*>& moveCompRegistries = m_ComponentsRegistries.Get(type.Id);
        for (ComponentRegistry* moveCompRegistry : moveCompRegistries)
        {
            if (registry == moveCompRegistry)
                continue;

            if (moveCompRegistry->GetComponent(entityId, type.Id))
            {
                // Don't rip components out if there is another component type that registry doesn't support
                if (!registry->SupportsAllComponents(moveCompRegistry->GetComponentTypes()))
                    return false;

                result.Add(moveCompRegistry);
                break;
            }
        }
    }
    if (result.Count() != types.Count())
        return false;
    return true;
}

void Scene::MoveComponentsToRegistry(uint64 entityId, ComponentRegistry* from, ComponentRegistry* to)
{
    const byte* fromData = from->GetComponents(entityId);
    if (!fromData)
        return;

    for (const ComponentType& compType : from->GetComponentTypes())
    {
        byte* toData = to->GetComponent(entityId, compType.Id);
        Crt::Copy(toData, fromData, compType.Size);
    }
    from->RemoveWithoutDestructor(entityId);
}
