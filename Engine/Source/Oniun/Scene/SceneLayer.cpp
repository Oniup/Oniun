#include "Oniun/Scene/SceneLayer.h"

SceneLayer::SceneLayer()
    : m_Active(nullptr)
{
}

Scene* SceneLayer::GetLoadedScene(const StringView& name) const
{
    for (Scene& scene : m_Loaded)
    {
        if (scene.GetTitle() == name)
            return &scene;
    }
    return nullptr;
}

void SceneLayer::OnUpdate()
{
    // TODO: Iterate through all systems and call their overrides
}
