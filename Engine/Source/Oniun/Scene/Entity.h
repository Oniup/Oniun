#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Scene/EntityEntry.h"
#include "Oniun/Scene/Scene.h"

namespace Oniun
{
    class Entity
    {
    public:
        static Entity Invalid;

    public:
        Entity();
        Entity(UUID id, Scene* scene);

    public:
        FORCE_INLINE operator UUID() const
        {
            return m_Id;
        }

        FORCE_INLINE bool operator==(const Entity& entity) const
        {
            return m_Id == entity.m_Id && m_Scene == entity.m_Scene;
        }

        FORCE_INLINE bool operator!=(const Entity& entity) const
        {
            return m_Id != entity.m_Id && m_Scene != entity.m_Scene;
        }

    public:
        StringView GetName() const;
        String GetFullName() const;
        uint64 GetNameId() const;
        UUID GetId() const;
        Entity GetParent() const;
        Entity GetFirstChild() const;
        Entity GetNextSibling() const;
        Array<Entity> GetChildren() const;

        FORCE_INLINE EntityEntry* GetEntry()
        {
            return &m_Scene->m_Entities.At(m_Id);
        }

        FORCE_INLINE const EntityEntry* GetEntry() const
        {
            return &m_Scene->m_Entities.At(m_Id);
        }

    public:
        bool HasChildren() const;
        bool HasSiblings() const;
        bool IsAlive() const;

        void Rename(const StringView& name);
        void Destroy();

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

    String ToString(const Entity& entity, bool fullNames = true);

    template <>
    struct Formatter<Entity>
    {
        bool FullNames = true;

        bool Parse(const FormatArgsContext& context);

        /// Formats the entity into a string representing the children as a tree.
        /// Options: "sm": Use smaller names
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
        void FormatTo(String& dest, const Entity& entity);

    private:
        void EntityToString(String& dest, const Entity& entity, uint64 depth, String& prefix);
    };
}
