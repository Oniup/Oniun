#include "Oniun/Scene/Entity.h"

#include "Oniun/Core/Math/Math.h"

Entity::Entity(Scene* scene, uint64 id)
    : m_Id(id), m_Scene(scene)
{
}
