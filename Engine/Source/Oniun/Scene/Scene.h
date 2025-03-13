#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Scene/ComponentRegistry.h"
#include "Oniun/Serialization/TypeInfo.h"

class Entity;

class Scene
{
public:
    Scene(const StringView& name = "Empty Scene");

    FORCE_INLINE const String& GetName() const
    {
        return m_Name;
    }

    Entity Create();
    void Destroy(uint64 entityId);
    bool IsEntityAlive(uint64 entityId) const;

    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs&&... args)
    {
    }

    template <typename... TComponents>
    void AddComponents(TComponents&&... components)
    {
    }

    template <typename TComponent>
    TComponent* GetComponent()
    {
        return nullptr;
    }

    template <typename TComponentTypes>
    void AddComponentRegistry()
    {
    }

private:
    String m_Name;
};
