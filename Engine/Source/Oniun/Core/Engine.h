#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"

namespace Oniun
{
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
        CommandLineArguments CmdLineArgs;
        Version AppBuild;
        Version EngineBuild;
        PlatformType PlatformType;

        AppInfo();
        AppInfo(const StringView& name, const CommandLineArguments& args, const Version& appBuild);
    };

    String ToString(const AppInfo::Version& version);

    class Engine
    {
    public:
        Engine();
        virtual ~Engine();

    public:
        FORCE_INLINE static void Quit()
        {
            m_Instance->m_Running = false;
        }

        FORCE_INLINE static const AppInfo& GetAppInfo()
        {
            return m_Instance->m_Info;
        }

        virtual void Setup() = 0;
        void Run();

        template <typename TLayer, typename... TArgs>
        FORCE_INLINE TLayer* RegisterLayer(TArgs&&... args)
        {
            return static_cast<TLayer*>(m_Instance->ImplRegisterLayer(Memory::New<TLayer>(args...)));
        }

        template <typename TLayer>
        FORCE_INLINE TLayer* RegisterLayer(TLayer* layer)
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

    protected:
        void RegisterAppInfo(const AppInfo& info);

    private:
        EngineLayer* ImplRegisterLayer(EngineLayer* layer);
        EngineLayer* ImplGetLayer(uint64 fastId);

    private:
        static Engine* m_Instance;

        Array<EngineLayer*> m_Layers;
        AppInfo m_Info;
        bool m_Running;

    protected:
        Logger m_Logger;
    };

    extern Engine* CreateApplication(const CommandLineArguments& args);
}
