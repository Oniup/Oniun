#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Scene/Scene.h"

class Entity
{
    friend Scene;

public:
    Entity();
    Entity(Scene::EntityName* name, Scene::EntityEntry* entry, Scene* scene);
    Entity(const Scene::EntityName* name, const Scene::EntityEntry* entry, const Scene* scene);

public:
    StringView GetName() const;
    String GetFullName() const;
    uint64 GetNameId() const;
    uint64 GetId() const;
    Entity GetParent() const;
    Entity GetFirstChild() const;
    Entity GetNextSibling() const;

    bool HasChildren() const;
    bool HasSiblings() const;

    bool IsValid() const;
    bool IsAlive() const;

    Entity AddChild(const StringView& name = "Child Entity");

    /// Removes a child entity from this entity, does not destroy child entity.
    /// @note This only removes the first found entity, so if there are more than one entity with duplicate names, then
    ///       you'll have to call this function multiple times.
    ///
    /// @param name  Entity with the exact name to remove.
    /// @param exact Check if the name includes the numbers of duplicate entity names.
    /// @return The child entity that was removed and now is parentless.
    Entity RemoveChild(const StringView& name, bool exact = false);

    template <typename TComponent, typename... TArgs>
    TComponent* Add(TArgs&&... args);

private:
    Scene::EntityName* m_Name;
    Scene::EntityEntry* m_Entry;
    Scene* m_Scene;
};

template <typename TComponent, typename ... TArgs>
TComponent* Entity::Add(TArgs&&... args)
{
    DEBUG_ASSERT(m_Entry);
    return m_Scene->AddComponent<TComponent>(*m_Entry, Memory::Forward<TArgs>(args)...);
}

/// Formats the entity into a string representing the children as a tree
/// Entity
///  ├ Head
///  └ Upper Torso
///     ├ Left Arm
///     │  └ Hand
///     ├ Right Arm
///     │  └ Hand
///     └ Hips
///        ├ Left Leg
///        │  └ Foot
///        └ Right Leg
///           └ Foot
String ToString(const Entity& entity, bool fullNames = false);
