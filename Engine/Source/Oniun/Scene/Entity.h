#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Scene/Scene.h"

class Entity
{
public:
    Entity(Scene* scene, uint64 id);

public:
    FORCE_INLINE uint64 GetId() const
    {
        return m_Id;
    }

    FORCE_INLINE Scene* GetScene() const
    {
        return m_Scene;
    }

    bool IsValid()
    {
        // TODO: ...
        return false;
    }

private:
    uint64 m_Id;
    Scene* m_Scene;
};
