#pragma once

#include "Oniun/Core/ClassConstraints.h"
#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"

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
    Version AppBuild;
    Version EngineBuild;
};

class Engine : public Singleton<Engine>
{
    friend Singleton;

private:
    Array<EngineLayer*> m_Layers;
    AppInfo m_Info;
    bool m_Running;

public:
    FORCE_INLINE static void Initialize(const AppInfo& appInfo)
    {
        Instance()->ImplInitialize(appInfo);
    }

    FORCE_INLINE static void Terminate()
    {
        Instance()->ImplTerminate();
    }

    FORCE_INLINE static void Run()
    {
        Instance()->ImplRun();
    }

    FORCE_INLINE static void Quit()
    {
        Instance()->m_Running = false;
    }

    FORCE_INLINE static const AppInfo& GetAppInfo()
    {
        return Instance()->m_Info;
    }

    template <typename TLayer, typename... TArgs>
    FORCE_INLINE static TLayer* RegisterLayer(TArgs&&... args)
    {
        return static_cast<TLayer*>(Instance()->ImplRegisterLayer(Memory::Allocate<TLayer>(args...)));
    }

    template <typename TLayer>
    FORCE_INLINE static TLayer* RegisterLayer(TLayer* layer)
    {
        return static_cast<TLayer*>(Instance()->ImplRegisterLayer(layer));
    }

    template <typename TLayer>
    FORCE_INLINE static TLayer* GetLayer()
    {
        uint64 id = TypeInfo::GetFastId<TLayer>();
        EngineLayer* layer = Instance()->ImplGetLayer(id);

        if (layer)
            return static_cast<TLayer*>(layer);

        LOG(Warning, TEXT("Failed to find engine layer '{}' with fast id: {}"), TypeInfo::GetInfo<TLayer>().Name, id);
        return nullptr;
    }

private:
    Engine();

    void ImplInitialize(const AppInfo& appInfo);
    void ImplRun();
    void ImplTerminate();

    EngineLayer* ImplRegisterLayer(EngineLayer* layer);
    EngineLayer* ImplGetLayer(uint64 fastId);
};
