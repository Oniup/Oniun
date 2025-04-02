#include "Core/Application.h"

// Editor core windows
#include "GuiWindows/Console.h"
#include "GuiWindows/ContentBrowser.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"
#include "GuiWindows/Viewport.h"
// Core engine layers
#include "Oniun/Core/EntryPoint.h"
#include "Oniun/Event/Event.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/RHI/ImGuiLayer.h"
#include "Oniun/Scene/SceneLayer.h"

#include "Oniun/Platform/FileSystem.h"

// Scene test
#include "Oniun/Scene/ComponentQuery.h"
#include "Oniun/Scene/Entity.h"
#include "Oniun/Scene/Components/Transform.h"
// Event test
#include "GuiWindows/Inspector.h"
#include "Oniun/Core/Input.h"
#include "Oniun/Event/KeyboardEvents.h"

#include "Oniun/RHI/Shader.h"

namespace Oniun::Editor
{
    void TestShader()
    {
        Array<Pair<RHI::ShaderType, String>> sources;
        sources.Add(RHI::Shader::GetSourceFromFile(StringView(ONIUN_RESOURCE_DIRECTORY "/Engine/Shaders/Phong.vert")));
        sources.Add(RHI::Shader::GetSourceFromFile(StringView(ONIUN_RESOURCE_DIRECTORY "/Engine/Shaders/Phong.frag")));
        for (auto&[type, src] : sources)
            LOG(Info, "{}:\n{}", type, src);

        RHI::Shader shader(RHI::Shader::CreateFromSource(sources));
        shader.Destroy();
    }

    void SetupTestScene()
    {
        Scene* scene = Engine::GetLayer<SceneLayer>()->LoadScene();
        scene->SetTitle("Test Scene");

        // Create entities
        Entity player = scene->Add("Player");
        player.Add<TransformComponent>();

        Entity entity = scene->Add("Ground");
        entity.Add<TransformComponent>();

        entity = scene->Add("Directional light");
        entity.Add<TransformComponent>();

        entity = scene->Add("Pickup");
        entity.Add<TransformComponent>();
    }

    class PrintMessage
    {
    public:
        void Print(IEvent* event, void* sender)
        {
            KeyPressedEvent* evt = static_cast<KeyPressedEvent*>(event);
            if (evt->GetKeyCode() == KeyCode::F3 && !evt->IsRepeated())
            {
                static LogType type = LogType::Info;
                ++(uint64&)(type);
                if (type > LogType::Error)
                    type = LogType::Verbose;

                Logger::Write(type, __FILE__, __FUNCTION__, __LINE__, "Debug Message");
            }
        }
    };

    Application::Application(const CommandLineArguments& args)
    {
        RegisterAppInfo(AppInfo("Oniun Engine", args,
                                AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH)));
    }

    void Application::Setup()
    {
        SetupCoreLayers();
        SetupGuiWindows();

        {
            static PrintMessage object;
            OnEventCallback callback;
            callback.Bind<PrintMessage, &PrintMessage::Print>(&object);
            EventDispatcher::AddListener<KeyPressedEvent>(callback);
        }
        SetupTestScene();
        TestShader();
    }

    void Application::SetupCoreLayers()
    {
#ifndef NDEBUG
        Logger::AddOutput(Memory::New<TerminalLogOutput>());
#endif
        Logger::AddOutput(Memory::New<FileLogOutput>("OniunEditor.log"));

        RegisterLayer<EventDispatcher>();
        RegisterLayer<Input>();

        RendererLayer* renderer = RegisterLayer<RendererLayer>(Fmt::Format("{} {}", GetAppInfo().Name, ONU_VERSION_STR), -1, -1,
                                     Window::DefaultFlags);
        renderer->GetWindow()->Maximize();

        RegisterLayer<SceneLayer>();
    }

    void Application::SetupGuiWindows()
    {
        // TODO: Config option for setting and loading other imgui layouts
        if (!FileSystem::FileExists("imgui.ini"))
        {
            FileSystem::CopyFile("imgui.ini", ONIUN_RESOURCE_DIRECTORY "/Editor/Layouts/Default.ini", true);
        }
        ImGuiLayer* imGui = RegisterLayer<ImGuiLayer>();
        imGui->Register(Memory::New<DockingSpace>());
        imGui->Register(Memory::New<Console>());
        imGui->Register(Memory::New<ContentBrowser>());
        // imGui->Register(Memory::New<Viewport>());
        Hierarchy* hierarchy = static_cast<Hierarchy*>(imGui->Register(Memory::New<Hierarchy>()));
        imGui->Register(Memory::New<Inspector>(hierarchy));
    }
}

namespace Oniun
{
    Engine* CreateApplication(const CommandLineArguments& args)
    {
        return Memory::New<Editor::Application>(args);
    }
}
