#include "Oniun/Core/Engine.h"

#include <GLFW/glfw3.h>

#include "Oniun/Platform/FileSystem.h"

Engine* Engine::m_Instance = nullptr;

Engine::Engine(const AppInfo& appInfo)
    : m_Info(appInfo), m_Running(true)
{
    ASSERT(!m_Instance && "Cannot have multiple instances of the engine");

    glfwInit();
    m_Instance = this;
}

Engine::~Engine()
{
    for (uint64 i = m_Layers.Count(); i > 0; --i)
        Memory::Free(m_Layers[i - 1]);

    glfwTerminate();
    m_Instance = nullptr;
}

void Engine::Run()
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
