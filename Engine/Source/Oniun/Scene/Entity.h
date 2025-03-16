#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Scene/Scene.h"

class Entity
{
public:
    FORCE_INLINE Entity()
        : m_Id(0), m_Scene(nullptr)
    {
    }

    FORCE_INLINE Entity(Scene* scene, uint64 id)
        : m_Id(id), m_Scene(scene)
    {
    }

public:
    FORCE_INLINE operator uint64() const
    {
        return m_Id;
    }

    FORCE_INLINE uint64 GetId() const
    {
        return m_Id;
    }

    FORCE_INLINE Scene* GetScene() const
    {
        return m_Scene;
    }

     FORCE_INLINE bool IsAlive()
     {
         return m_Scene && m_Scene->IsEntityAlive(m_Id);
     }

     template <typename TComponent, typename... TArgs>
     FORCE_INLINE void AddComponent(TArgs&&... args)
     {
         m_Scene->AddComponent<TComponent>(m_Id, Memory::Forward<TArgs>(args)...);
     }

     template <typename TComponent>
     FORCE_INLINE TComponent* GetComponent()
     {
         return m_Scene->GetComponent<TComponent>(m_Id);
     }

private:
    uint64 m_Id;
    Scene* m_Scene;
};
