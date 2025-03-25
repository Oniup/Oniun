#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Scene/Scene.h"

namespace Oniun
{
    class SceneLayer : public EngineLayer
    {
        API_ENGINE_LAYER()

        friend Scene;

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

        // static void SetCreateEntityCallback(Function<void(Entity entity)>&& callback);
        // static void SetDestroyEntityCallback(Function<void(Entity entity)>&& callback);

    public:
        void OnUpdate() override;

    private:
        static uint64 m_ChunksPerBlockCount;
        // static Function<void(Entity entity)> m_CreateEntityCallback;
        // static Function<void(Entity entity)> m_DestroyEntityCallback;

        Array<Scene> m_Loaded;
        Scene* m_Active;
    };
}
