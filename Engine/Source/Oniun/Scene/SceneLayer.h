#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Scene/Scene.h"

namespace Oniun
{
    class SceneLayer : public EngineLayer
    {
        API_ENGINE_LAYER()

    public:
        SceneLayer(uint64 componentChunksPerBlockCount = DEFAULT_COMPONENT_POOL_CHUNK_PER_BLOCK_COUNT);

    public:
        FORCE_INLINE static uint64 GetComponentPoolChunksPerBlockCount()
        {
            return m_ChunksPerBlockCount;
        }

        FORCE_INLINE Array<Scene>& GetLoadedScenes()
        {
            return m_Loaded;
        }

        FORCE_INLINE const Array<Scene>& GetLoadedScenes() const
        {
            return m_Loaded;
        }

        Scene* GetLoadedScene(const StringView& title) const;

    public:
        Scene* LoadScene(const StringView& resourcePath = nullptr);
        void UnloadScene(Scene*& scene);

    public:
        void OnUpdate() override;

    private:
        static uint64 m_ChunksPerBlockCount;

        Array<Scene> m_Loaded;
    };
}
