#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Scene/Scene.h"

class SceneLayer : public EngineLayer
{
public:
    SceneLayer();

public:
    FORCE_INLINE Scene* GetActiveScene()
    {
        return m_Active;
    }

    FORCE_INLINE const Scene* GetActiveScene() const
    {
        return m_Active;
    }

    Scene* GetLoadedScene(const StringView& name) const;

    void OnUpdate() override;

private:
    Array<Scene> m_Loaded;
    Scene* m_Active;
};
