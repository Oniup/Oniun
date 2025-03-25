#include "Oniun/Scene/SceneLayer.h"

namespace Oniun
{
    uint64 SceneLayer::m_ChunksPerBlockCount = 0;

    SceneLayer::SceneLayer(uint64 componentChunksPerBlockCount)
        : m_Active(nullptr)
    {
        m_ChunksPerBlockCount = componentChunksPerBlockCount;
    }

    Scene* SceneLayer::GetLoadedScene(const StringView& title) const
    {
        for (Scene& scene : m_Loaded)
        {
            if (scene.GetTitle() == title)
                return &scene;
        }
        return nullptr;
    }

    // void SceneLayer::SetCreateEntityCallback(Function<void(Entity entity)>&& callback)
    // {
    //     m_CreateEntityCallback = Memory::Move(callback);
    // }
    //
    // void SceneLayer::SetDestroyEntityCallback(Function<void(Entity entity)>&& callback)
    // {
    //     m_DestroyEntityCallback = Memory::Move(callback);
    // }

    void SceneLayer::OnUpdate()
    {
        // TODO: Iterate through all systems and call their overrides
    }
}
