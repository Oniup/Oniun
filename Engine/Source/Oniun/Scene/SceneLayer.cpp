#include "Oniun/Scene/SceneLayer.h"

#include "Oniun/Core/Logger.h"

namespace Oniun
{
    uint64 SceneLayer::m_ChunksPerBlockCount = 0;

    SceneLayer::SceneLayer(uint64 componentChunksPerBlockCount)
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

    Scene* SceneLayer::LoadScene(const StringView& resourcePath)
    {
        if (resourcePath.IsEmpty())
            m_Loaded.Add(Memory::Move(Scene()));
        else
        {
            // TODO: Find scene resource file and load ...
            return nullptr;
        }
        return &m_Loaded.Last();
    }

    void SceneLayer::UnloadScene(Scene*& scene)
    {
        if (scene != nullptr)
        {
            m_Loaded.Remove(*scene);
            scene = nullptr;
        }
    }

    void SceneLayer::OnUpdate()
    {
        // TODO: Iterate through all systems and call their overrides
    }
}
