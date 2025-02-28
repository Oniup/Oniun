#include "Oniun/Core/Engine.h"

#include <GLFW/glfw3.h>

Engine::Engine()
    : m_Running(true)
{
}

Engine::~Engine()
{
    for (EngineLayer* layer : m_Layers)
        Memory::Free(layer);
}

void Engine::ImplInitialize(const AppInfo& appInfo)
{
    m_Info = appInfo;
}

void Engine::ImplRun()
{
    while (m_Running)
    {
        for (EngineLayer* layer : m_Layers)
            layer->OnUpdate();
    }
}

EngineLayer* Engine::ImplRegisterLayer(EngineLayer* layer)
{
    m_Layers.Add(layer);
    m_Layers.Last()->OnStart();
    return m_Layers.Last();
}
