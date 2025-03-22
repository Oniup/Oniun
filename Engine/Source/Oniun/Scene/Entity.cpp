#include "Oniun.pch.h"
#include "Oniun/Scene/Entity.h"

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
    DEBUG_ASSERT(IsAlive());
    return GetEntry()->Name.Data();
}

String Entity::GetFullName() const
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->NameId > 0)
        return Format("{}{}", GetEntry()->Name.Data(), GetEntry()->NameId);
    return GetEntry()->Name.Data();
}

uint64 Entity::GetNameId() const
{
    DEBUG_ASSERT(IsAlive());
    return GetEntry()->NameId;
}

uint64 Entity::GetId() const
{
    DEBUG_ASSERT(IsAlive());
    return GetEntry()->GetId();
}

Entity Entity::GetParent() const
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->Parent != NO_POS)
        return m_Scene->Find(GetEntry()->Parent);
    return Entity();
}

Entity Entity::GetFirstChild() const
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->FirstChild != NO_POS)
        return m_Scene->Find(GetEntry()->FirstChild);
    return Entity();
}

Entity Entity::GetNextSibling() const
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->Next != NO_POS)
        return m_Scene->Find(GetEntry()->Next);
    return Entity();
}

bool Entity::HasChildren() const
{
    if (!IsAlive())
        return false;
    return GetEntry()->FirstChild != NO_POS;
}

bool Entity::HasSiblings() const
{
    if (!IsAlive())
        return false;
    return GetEntry()->Next != NO_POS;
}

bool Entity::IsAlive() const
{
    return m_Id != NO_POS && m_Scene->IsAlive(*this);
}

Entity Entity::AddChild(const StringView& name)
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->FirstChild != NO_POS)
    {
        Entity child = m_Scene->Find(GetEntry()->FirstChild);
        while (child.GetEntry()->Next != NO_POS)
            child = m_Scene->Find(child.GetEntry()->Next);

        Entity newChild = m_Scene->Add(name);
        newChild.GetEntry()->Parent = m_Id;
        child.GetEntry()->Next = newChild.m_Id;
        return newChild;
    }

    Entity child = m_Scene->Add(name);
    child.GetEntry()->Parent = m_Id;
    GetEntry()->FirstChild = child.m_Id;
    return child;
}

Entity Entity::RemoveChild(const StringView& name, bool exact)
{
    DEBUG_ASSERT(IsAlive());
    if (GetEntry()->FirstChild != NO_POS)
        return Invalid;

    // Find child
    Entity previous;
    Entity child = m_Scene->Find(GetEntry()->FirstChild);
    while (child.IsAlive())
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
    if (child.IsAlive())
    {
        if (previous.IsAlive())
            previous.GetEntry()->Next = child.GetEntry()->Next;
        else
            GetEntry()->FirstChild = NO_POS;
        child.GetEntry()->Parent = NO_POS;
    }
    return child;
}

void EntityToString(String& result, const Entity& entity, bool fullNames, uint64 depth, String& prefix)
{
    if (!entity.IsAlive())
        return;

    // Get entity name
    char buffer[Scene::MaxEntityFullNameSize];
    if (fullNames && entity.GetNameId() > 0)
        Crt::Format(buffer, Scene::MaxEntityFullNameSize, "%s%llu", *entity.GetName(), entity.GetNameId());
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
    while (child.IsAlive())
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
