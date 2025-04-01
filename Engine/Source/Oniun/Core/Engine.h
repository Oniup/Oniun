#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"

namespace Oniun
{
    /// @brief Struct representing command line arguments.
    ///
    /// The CommandLineArguments struct holds the count and the list of command line arguments passed to the
    /// application.
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

    /// @brief Base class for the engine application. This is responsible for managing the application's lifecycle, and
    ///        managing engine layers (EngineLayer).
    ///
    /// @warning Only add engine layers within the Setup() override otherwise you risk dangling pointer errors.
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

        /// @brief Registers an engine layer.
        ///
        /// @tparam TLayer The type of the layer to register.
        /// @tparam TArgs  The types of the arguments to pass to the layer's constructor.
        /// @param args    The arguments to pass to the layer's constructor.
        /// @return A pointer to the newly registered layer.
        template <typename TLayer, typename... TArgs>
        FORCE_INLINE TLayer* RegisterLayer(TArgs&&... args)
        {
            return static_cast<TLayer*>(m_Instance->ImplRegisterLayer(Memory::New<TLayer>(args...)));
        }

        /// @brief Registers an engine layer.
        ///
        /// @tparam TLayer The type of the layer to register.
        /// @param layer   A pointer to the layer to register.
        /// @return A pointer to the newly registered layer.
        template <typename TLayer>
        FORCE_INLINE TLayer* RegisterLayer(TLayer* layer)
        {
            return static_cast<TLayer*>(m_Instance->ImplRegisterLayer(layer));
        }

        /// @brief Gets a registered engine layer.
        ///
        /// @tparam TLayer The type of the layer to get.
        /// @return A pointer to the registered layer, or nullptr if the layer was not found.
        template <typename TLayer>
        FORCE_INLINE static TLayer* GetLayer()
        {
            constexpr uint64 id = TypeInfo::GetFastId<TLayer>();
            EngineLayer* layer = m_Instance->ImplGetLayer(id);

            if (layer)
                return static_cast<TLayer*>(layer);

            LOG(Warning, "Failed to find engine layer '{}' with fast id: {}", TypeInfo::GetInfo<TLayer>().Name, id);
            return nullptr;
        }

    protected:
        /// @brief Registers application information.
        ///
        /// @param info The application information to register.
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

    /// @brief Creates an application instance.
    ///
    /// This function should be implemented by the application to create and return an instance of the Engine-derived
    /// class.
    ///
    /// @param args The command line arguments.
    /// @return A pointer to the created application instance.
    extern Engine* CreateApplication(const CommandLineArguments& args);

    template <>
    struct Formatter<AppInfo::Version>
    {
        FORMATTER_DEFAULT_PARSE_FUNC()
        void FormatTo(String& ctx, const AppInfo::Version& version);
    };
}
