#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Scene/Scene.h"

namespace Oniun
{
    class Entity
    {
        friend Scene;

    public:
        static Entity Invalid;

    public:
        Entity();
        Entity(UUID id, Scene* scene);

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

        template <typename TComponent>
        TComponent* Get();

    private:
        FORCE_INLINE Scene::EntityEntry* GetEntry()
        {
            return &m_Scene->m_Entities.At(m_Id);
        }

        FORCE_INLINE const Scene::EntityEntry* GetEntry() const
        {
            return &m_Scene->m_Entities.At(m_Id);
        }

    private:
        UUID m_Id;
        Scene* m_Scene;
    };

    template <typename TComponent, typename ... TArgs>
    TComponent* Entity::Add(TArgs&&... args)
    {
        return m_Scene->AddComponent<TComponent>(m_Id, Memory::Forward<TArgs>(args)...);
    }

    template <typename TComponent>
    TComponent* Entity::Get()
    {
        return m_Scene->GetComponent<TComponent>(m_Id);
    }

    /// Formats the entity into a string representing the children as a tree.
    ///
    /// Resulting string should look similar to the following:
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
    ///
    /// @param entity    Target entity to convert to a string.
    /// @param fullNames Should include the unique number. (e.g. "Entity (123)").
    /// @return A string version of the entity
    String ToString(const Entity& entity, bool fullNames = true);
}
