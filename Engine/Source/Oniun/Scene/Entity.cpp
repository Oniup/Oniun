#include "Oniun/Scene/Entity.h"

namespace Oniun
{
    Entity Entity::Invalid;

    Entity::Entity()
        : m_Id(NO_POS), m_Scene(nullptr)
    {
    }

    Entity::Entity(UUID id, Scene* scene)
        : m_Id(id), m_Scene(scene)
    {
    }

    StringView Entity::GetName() const
    {
        ASSERT(IsAlive());
        return GetEntry()->Name.Data();
    }

    String Entity::GetFullName() const
    {
        ASSERT(IsAlive());
        const EntityEntry* entry = GetEntry();
        if (entry->NameId > 0)
            return Format("{}{}", entry->Name.Data(), entry->NameId);
        return entry->Name.Data();
    }

    uint64 Entity::GetNameId() const
    {
        ASSERT(IsAlive());
        return GetEntry()->NameId;
    }

    uint64 Entity::GetId() const
    {
        ASSERT(IsAlive());
        return GetEntry()->GetId();
    }

    Entity Entity::GetParent() const
    {
        ASSERT(IsAlive());
        const EntityEntry* entry = GetEntry();
        if (entry->Parent != NO_POS)
            return m_Scene->Find(entry->Parent);
        return Invalid;
    }

    Entity Entity::GetFirstChild() const
    {
        ASSERT(IsAlive());
        const EntityEntry* entry = GetEntry();
        if (entry->FirstChild != NO_POS)
            return m_Scene->Find(entry->FirstChild);
        return Invalid;
    }

    Entity Entity::GetNextSibling() const
    {
        ASSERT(IsAlive());
        const EntityEntry* entry = GetEntry();
        if (entry->Next != NO_POS)
            return m_Scene->Find(entry->Next);
        return Invalid;
    }

    Array<Entity> Entity::GetChildren() const
    {
        ASSERT(IsAlive());
        Entity child = GetFirstChild();
        Array<Entity> children;
        while (child != Invalid)
        {
            children.Add(child);
            child = child.GetNextSibling();
        }
        return children;
    }

    bool Entity::HasChildren() const
    {
        ASSERT(IsAlive());
        return GetEntry()->FirstChild != NO_POS;
    }

    bool Entity::HasSiblings() const
    {
        ASSERT(IsAlive());
        return GetEntry()->Next != NO_POS;
    }

    bool Entity::IsAlive() const
    {
        return m_Id != NO_POS && m_Scene->IsAlive(*this);
    }

    void Entity::Rename(const StringView& name)
    {
        ASSERT(IsAlive());
        m_Scene->RenameEntity(*this);
    }

    void Entity::Destroy()
    {
        m_Scene->Remove(*this);
        m_Id = NO_POS;
        m_Scene = nullptr;
    }

    Entity Entity::AddChild(const StringView& name)
    {
        ASSERT(IsAlive());
        EntityEntry* entry = GetEntry();
        if (entry->FirstChild != NO_POS)
        {
            Entity child = m_Scene->Find(entry->FirstChild);
            while (child.GetEntry()->Next != NO_POS)
                child = m_Scene->Find(child.GetEntry()->Next);

            Entity newChild = m_Scene->Add(name);
            newChild.GetEntry()->Parent = m_Id;
            child.GetEntry()->Next = newChild.m_Id;
            return newChild;
        }

        Entity child = m_Scene->Add(name);
        child.GetEntry()->Parent = m_Id;
        entry->FirstChild = child.m_Id;
        return child;
    }

    Entity Entity::RemoveChild(const StringView& name, bool exact)
    {
        ASSERT(IsAlive());
        EntityEntry* entry = GetEntry();
        if (entry->FirstChild != NO_POS)
            return Invalid;

        // Find child
        Entity previous;
        Entity child = m_Scene->Find(entry->FirstChild);
        while (child != Invalid)
        {
            if (exact && child.GetEntry()->NameId > 0)
            {
                if (child.GetFullName().Compare(name))
                    break;
            }
            else
            {
                if (child.GetName().Compare(name))
                    break;
            }
            previous = child;
            child = child.GetNextSibling();
        }
        // Remove child from entity without destroying it
        if (child != Invalid)
        {
            if (previous.IsAlive())
                previous.GetEntry()->Next = child.GetEntry()->Next;
            else
                entry->FirstChild = NO_POS;
            child.GetEntry()->Parent = NO_POS;
        }
        return child;
    }

    void EntityToString(String& result, const Entity& entity, bool fullNames, uint64 depth, String& prefix)
    {
        if (entity != Entity::Invalid)
            return;

        // Get entity name
        char buffer[EntityEntry::MaxFullNameSize];
        if (fullNames && entity.GetNameId() > 0)
            Crt::Format(buffer, EntityEntry::MaxFullNameSize, "%s%llu", *entity.GetName(), entity.GetNameId());
        else
            Crt::Format(buffer, EntityEntry::MaxFullNameSize, "%s", *entity.GetName());

        result.Append(prefix);
        result.Append(buffer);
        result.Append("\n");
        prefix.Replace("└", " ");
        prefix.Replace("├", "│");

        // Collect children
        Array<Entity> children = entity.GetChildren();

        // Process children with correct formatting
        for (size_t i = 0; i < children.Count(); ++i)
        {
            bool isLast = (i == children.Count() - 1);
            String childPrefix = prefix + (isLast ? "└ " : "├ ");
            EntityToString(result, children[i], fullNames, depth + 1, childPrefix);
        }
    }

    String ToString(const Entity& entity, bool fullNames)
    {
        String result;
        if (entity.IsAlive())
        {
            String prefix("");
            EntityToString(result, entity, fullNames, 0, prefix);
        }
        return result;
    }
}
