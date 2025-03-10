#pragma once

#include "Oniun/Scene/Query.h"

ComponentQuery::ComponentQuery(const uint64* ids, uint64 count,
    Array<ComponentRegistry*>&& registries)
    : m_ComponentIds(ids, count), m_Registries(Memory::Move(registries))
{
}
