#include "Oniun/Scene/Scene.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Math.h"
#include "Oniun/Scene/Entity.h"

Scene::Scene(const StringView& name)
    : m_Name(name)
{
}
