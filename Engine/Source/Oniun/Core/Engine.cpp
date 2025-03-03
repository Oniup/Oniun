#include "Oniun/Core/Engine.h"

#include <GLFW/glfw3.h>

#include "Oniun/Platform/FileSystem.h"

Engine::Engine()
    : m_Running(true)
{
    glfwInit();
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

void Engine::ImplTerminate()
{
    for (uint64 i = m_Layers.Count(); i > 0; --i)
        Memory::Free(m_Layers[i - 1]);

    glfwTerminate();
}

EngineLayer* Engine::ImplRegisterLayer(EngineLayer* layer)
{
    m_Layers.Add(layer);
    m_Layers.Last()->OnStart();
    return m_Layers.Last();
}

EngineLayer* Engine::ImplGetLayer(uint64 fastId)
{
    for (EngineLayer* layer : m_Layers)
    {
        if (layer->GetFastId() == fastId)
            return layer;
    }
    return nullptr;
}

String ToString(const AppInfo::Version& version)
{
    return Format("{}, {}, {}", version.Major, version.Minor, version.Patch);
}
