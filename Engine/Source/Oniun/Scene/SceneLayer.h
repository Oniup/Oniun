#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Scene/Scene.h"

class SceneLayer : public EngineLayer
{
    API_ENGINE_LAYER()

public:
    SceneLayer(uint64 componentChunksPerBlockCount = DEFAULT_COMPONENT_POOL_CHUNK_PER_BLOCK_COUNT);

public:
    FORCE_INLINE Scene* GetActiveScene()
    {
        return m_Active;
    }

    FORCE_INLINE const Scene* GetActiveScene() const
    {
        return m_Active;
    }

    FORCE_INLINE static uint64 GetComponentPoolChunksPerBlockCount()
    {
        return m_ChunksPerBlockCount;
    }

    Scene* GetLoadedScene(const StringView& title) const;

    void OnUpdate() override;

private:
    static uint64 m_ChunksPerBlockCount;
    Array<Scene> m_Loaded;
    Scene* m_Active;
};
