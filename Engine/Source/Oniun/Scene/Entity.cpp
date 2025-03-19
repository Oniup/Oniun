#include "Oniun.pch.h"
#include "Oniun/Scene/Entity.h"

Entity::Entity()
    : m_Name(nullptr), m_Entry(nullptr), m_Scene(nullptr)
{
}

Entity::Entity(Scene::EntityName* name, Scene::EntityEntry* entry, Scene* scene)
    : m_Name(name), m_Entry(entry), m_Scene(scene)
{
}

StringView Entity::GetName() const
{
    DEBUG_ASSERT(IsValid());
    return m_Name->Name.Data();
}

String Entity::GetFullName() const
{
    DEBUG_ASSERT(IsValid());
    if (m_Name->OffsetCounter > 0)
        return Format("{} ({})", m_Name->Name.Data(), m_Name->OffsetCounter);
    return m_Name->Name.Data();
}

uint64 Entity::GetNameId() const
{
    DEBUG_ASSERT(IsValid());
    return m_Name->OffsetCounter;
}

uint64 Entity::GetId() const
{
    DEBUG_ASSERT(IsValid());
    return Hash<Scene::EntityName>{}.Get(*m_Name);
}

Entity Entity::GetParent() const
{
    DEBUG_ASSERT(IsValid());
    if (m_Entry->Parent)
        return m_Scene->Find(*m_Entry->Parent);
    return Entity();
}

Entity Entity::GetFirstChild() const
{
    DEBUG_ASSERT(IsValid());
    if (m_Entry->FirstChild)
        return m_Scene->Find(*m_Entry->FirstChild);
    return Entity();
}

Entity Entity::GetNextSibling() const
{
    DEBUG_ASSERT(IsValid());
    if (m_Entry->Next)
        return m_Scene->Find(*m_Entry->Next);
    return Entity();
}

bool Entity::HasChildren() const
{
    if (!IsValid())
        return false;
    return m_Entry->FirstChild != nullptr;
}

bool Entity::HasSiblings() const
{
    if (!IsValid())
        return false;
    return m_Entry->Next != nullptr;
}

bool Entity::IsValid() const
{
    return m_Entry != nullptr;
}

bool Entity::IsAlive() const
{
    return m_Scene->IsAlive(*this);
}

Entity Entity::AddChild(const StringView& name)
{
    DEBUG_ASSERT(IsValid());
    if (m_Entry->FirstChild)
    {
        Entity child = m_Scene->Find(*m_Entry->FirstChild);
        while (child.m_Entry->Next)
            child = m_Scene->Find(*child.m_Entry->Next);

        Entity newChild = m_Scene->Add(name);
        newChild.m_Entry->Parent = m_Name;
        child.m_Entry->Next = newChild.m_Name;
        return newChild;
    }

    Entity child = m_Scene->Add(name);
    child.m_Entry->Parent = m_Name;
    m_Entry->FirstChild = child.m_Name;
    return child;
}

Entity Entity::RemoveChild(const StringView& name, bool exact)
{
    DEBUG_ASSERT(IsValid());
    if (!m_Entry->FirstChild)
        return Entity();

    // Find child
    Entity previous;
    Entity child = m_Scene->Find(*m_Entry->FirstChild);
    while (child.IsValid())
    {
        if (exact && child.m_Name->OffsetCounter > 0)
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
    // Remove child from entity
    if (child.IsValid())
    {
        if (previous.IsValid())
            previous.m_Entry->Next = child.m_Entry->Next;
        else
            m_Entry->FirstChild = nullptr;
        child.m_Entry->Parent = nullptr;
    }
    return child;
}

void EntityToString(String& result, const Entity& entity, bool fullNames, uint64 depth, String& prefix)
{
    if (!entity.IsValid())
        return;

    // Get entity name
    char buffer[Scene::MaxEntityFullNameSize];
    if (fullNames)
        Crt::Format(buffer, Scene::MaxEntityFullNameSize, "%s (%llu)", *entity.GetName(), entity.GetNameId());
    else
        Crt::Format(buffer, Scene::MaxEntityFullNameSize, "%s", *entity.GetName());

    result.Append(prefix);
    result.Append(buffer);
    result.Append("\n");
    prefix.Replace("└", " ");
    prefix.Replace("├", "│");

    // Collect children
    Entity child = entity.GetFirstChild();
    Array<Entity> children;
    while (child.IsValid())
    {
        children.Add(child);
        child = child.GetNextSibling();
    }

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
    String prefix("");
    EntityToString(result, entity, fullNames, 0, prefix);
    return result;
}
