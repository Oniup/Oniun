#pragma once

#include "Oniun/Core/ClassConstraints.h"
#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"

struct CommandLineArguments
{
    int32 Count;
    char** Args;

    char* operator[](int32 index)
    {
        ASSERT(index < Count);
        return Args[index];
    }
};

struct AppInfo
{
    struct Version
    {
        uint64 Major;
        uint64 Minor;
        uint64 Patch;

        Version()
            : Major(0), Minor(0), Patch(0)
        {
        }

        Version(uint64 major, uint64 minor, uint64 patch)
            : Major(major), Minor(minor), Patch(patch)
        {
        }
    };

    String Name;
    CommandLineArguments CommandLineArguments;
    Version AppBuild;
    Version EngineBuild;
};

class Engine
{
public:
    Engine(const AppInfo& appInfo);
    ~Engine();

public:
    FORCE_INLINE static void Quit()
    {
        m_Instance->m_Running = false;
    }

    FORCE_INLINE static const AppInfo& GetAppInfo()
    {
        return m_Instance->m_Info;
    }

    void Run();

    template <typename TLayer, typename... TArgs>
    FORCE_INLINE static TLayer* RegisterLayer(TArgs&&... args)
    {
        return static_cast<TLayer*>(m_Instance->ImplRegisterLayer(Memory::Allocate<TLayer>(args...)));
    }

    template <typename TLayer>
    FORCE_INLINE static TLayer* RegisterLayer(TLayer* layer)
    {
        return static_cast<TLayer*>(m_Instance->ImplRegisterLayer(layer));
    }

    template <typename TLayer>
    FORCE_INLINE static TLayer* GetLayer()
    {
        uint64 id = TypeInfo::GetFastId<TLayer>();
        EngineLayer* layer = m_Instance->ImplGetLayer(id);

        if (layer)
            return static_cast<TLayer*>(layer);

        LOG(Warning, "Failed to find engine layer '{}' with fast id: {}", TypeInfo::GetInfo<TLayer>().Name, id);
        return nullptr;
    }

private:
    EngineLayer* ImplRegisterLayer(EngineLayer* layer);
    EngineLayer* ImplGetLayer(uint64 fastId);

private:
    static Engine* m_Instance;

    Array<EngineLayer*> m_Layers;
    AppInfo m_Info;
    bool m_Running;

};

String ToString(const AppInfo::Version& version);
