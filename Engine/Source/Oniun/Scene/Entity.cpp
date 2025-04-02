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
            return Fmt::Format("{}{}", entry->Name.Data(), entry->NameId);
        return entry->Name.Data();
    }

    uint64 Entity::GetNameId() const
    {
        ASSERT(IsAlive());
        return GetEntry()->NameId;
    }

    UUID Entity::GetId() const
    {
        return m_Id;
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

    bool Formatter<Entity>::Parse(const FormatArgsContext& context)
    {
        for (StringView arg : context)
        {
            if (arg == "sm")
                FullNames = false;
        }
        return true;
    }

    void Formatter<Entity>::FormatTo(String& dest, const Entity& entity)
    {
        if (entity.IsAlive())
        {
            String prefix("");
            EntityToString(dest, entity, 0, prefix);
        }
    }

    void Formatter<Entity>::EntityToString(String& dest, const Entity& entity, uint64 depth, String& prefix)
    {
        if (entity != Entity::Invalid)
            return;

        // Get entity name
        char buffer[EntityEntry::MaxFullNameSize];
        if (FullNames && entity.GetNameId() > 0)
            CRT::Format(buffer, EntityEntry::MaxFullNameSize, "%s%llu", *entity.GetName(), entity.GetNameId());
        else
            CRT::Format(buffer, EntityEntry::MaxFullNameSize, "%s", *entity.GetName());

        dest.Append(prefix);
        dest.Append(buffer);
        dest.Append("\n");
        prefix.Replace("└", " ");
        prefix.Replace("├", "│");

        // Collect children
        Array<Entity> children = entity.GetChildren();

        // Process children with correct formatting
        for (size_t i = 0; i < children.Count(); ++i)
        {
            bool isLast = (i == children.Count() - 1);
            String childPrefix = prefix + (isLast ? "└ " : "├ ");
            EntityToString(dest, children[i], depth + 1, childPrefix);
        }
    }
}
