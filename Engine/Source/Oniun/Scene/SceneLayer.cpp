#include "Oniun.pch.h"
#include "Oniun/Scene/SceneLayer.h"

uint64 SceneLayer::m_ComponentRegistryChunksPerBlockCount = 0;

SceneLayer::SceneLayer(uint64 componentChunksPerBlockCount)
    : m_Active(nullptr)
{
    m_ComponentRegistryChunksPerBlockCount = componentChunksPerBlockCount;
}

Scene* SceneLayer::GetLoadedScene(const StringView& name) const
{
    for (Scene& scene : m_Loaded)
    {
        if (scene.GetName() == name)
            return &scene;
    }
    return nullptr;
}

void SceneLayer::OnUpdate()
{
    // TODO: Iterate through all systems and call their overrides
}
