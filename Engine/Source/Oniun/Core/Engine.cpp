#include "Oniun/Core/Engine.h"

#include <GLFW/glfw3.h>

namespace Oniun
{
    Engine* Engine::m_Instance = nullptr;

    AppInfo::AppInfo()
        : CmdLineArgs(), PlatformType()
    {
    }

    AppInfo::AppInfo(const StringView& name, const CommandLineArguments& args, const Version& appBuild)
        : Name(name), CmdLineArgs(args), AppBuild(appBuild),
          EngineBuild(Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH)), PlatformType(GetPlatformType())
    {
        // TODO: Platform type
    }

    String ToString(const AppInfo::Version& version)
    {
        return Format("{}.{}.{}", version.Major, version.Minor, version.Patch);
    }

    Engine::Engine()
        : m_Running(false)
    {
        ASSERT(!m_Instance && "Cannot have multiple instances of the engine");

        glfwInit();
        m_Instance = this;
    }

    Engine::~Engine()
    {
        for (uint64 i = m_Layers.Count(); i > 0; --i)
            Memory::Delete(m_Layers[i - 1]);

        glfwTerminate();
        m_Instance = nullptr;
    }

    void Engine::Run()
    {
        for (EngineLayer* layer : m_Layers)
            layer->OnStart();

        m_Running = true;
        while (m_Running)
        {
            for (EngineLayer* layer : m_Layers)
                layer->OnUpdate();
        }
    }

    void Engine::RegisterAppInfo(const AppInfo& info)
    {
        m_Info = info;
    }

    EngineLayer* Engine::ImplRegisterLayer(EngineLayer* layer)
    {
        ASSERT(!m_Running)
        m_Layers.Add(layer);
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
}
