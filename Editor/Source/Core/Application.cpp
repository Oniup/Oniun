#include "Core/Application.h"

// Main function that calls CreateApplication
#include <Oniun/Core/EntryPoint.h>

// Core engine layers
#include <Oniun/Event/Event.h>
#include <Oniun/Renderer/RendererLayer.h>
#include <Oniun/RHI/ImGuiLayer.h>
#include <Oniun/Scene/SceneLayer.h>

// Test
#include <Oniun/Core/Input.h>
#include <Oniun/Core/Math/Vector3.h>
#include <Oniun/Core/Math/Vector4.h>
#include <Oniun/Scene/ComponentQuery.h>
#include <Oniun/Scene/Entity.h>

#include <Oniun/Event/KeyboardEvents.h>

#include <Oniun/Core/String/Format.h>

// Editor core windows
#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

namespace Oniun::Editor
{
    struct TransformComponent
    {
        Vector3 Position;
        Vector3 Scale;
        Vector3 Rotation;
    };

    void SetupTestScene()
    {
        Scene* scene = Engine::GetLayer<SceneLayer>()->LoadScene();
        scene->SetTitle("Test Scene");

        // Create entities
        {
            Entity slimeBase = scene->Add("Slime");
            slimeBase.Add<TransformComponent>();
            Entity body = slimeBase.AddChild("Body");
            body.AddChild("Left Eye");
            body.AddChild("Right Eye");

            Entity treasure = slimeBase.AddChild("Treasure");
            treasure.AddChild("Weapon");
            Entity coinPouch = treasure.AddChild("Coin Pouch");
            coinPouch.AddChild("Coin");
            coinPouch.AddChild("Coin");
            coinPouch.AddChild("Coin");
            coinPouch.AddChild("Coin");
        }
        {
            Entity playerBase = scene->Add("Player");
            playerBase.Add<TransformComponent>();
            playerBase.AddChild("Head").Add<TransformComponent>();
            Entity torso = playerBase.AddChild("Torso");
            torso.Add<TransformComponent>();
            torso.AddChild("Left Arm").AddChild("Hand");
            torso.AddChild("Right Arm").AddChild("Hand");

            Entity hips = torso.AddChild("Hips");
            hips.Add<TransformComponent>();
            hips.AddChild("Left Leg").AddChild("Foot");
            hips.AddChild("Right Leg").AddChild("Foot");
        }
        {
            auto addTree = [](Entity& parent)
            {
                Entity tree = parent.AddChild("Tree");
                tree.Add<TransformComponent>();
                tree.AddChild("Trunk").AddChild("Leaves");
            };

            Entity environment = scene->Add("Environment");
            environment.AddChild("Floor");
            addTree(environment);
            addTree(environment);
            addTree(environment);
            addTree(environment);
        }
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

    template <typename T>
    void FormatterTest(const T& val, const StringView& insertStr, bool startOnNewLine = false)
    {
        Formatter<T> formatter;
        FormatArgsContext context(insertStr);
        formatter.Parse(context);
        String result;
        formatter.FormatTo(result, val);
        if (startOnNewLine)
            LOG(Info, "{} using \"{}\" =\n{}", TypeInfo::GetName<T>(), insertStr, result);
        else
            LOG(Info, "{} using \"{}\" = {}", TypeInfo::GetName<T>(), insertStr, result);
    }

    Application::Application(const CommandLineArguments& args)
    {
        RegisterAppInfo(AppInfo("Oniun Engine", args, AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH)));
    }

    void Application::Setup()
    {
        // Setup debug logger
#ifndef NDEBUG
        Logger::AddOutput(Memory::New<TerminalLogOutput>());
#endif
        Logger::AddOutput(Memory::New<FileLogOutput>("OniunEditor.log"));

        // Core engine layers
        RegisterLayer<EventDispatcher>();
        RegisterLayer<Input>();
        RegisterLayer<RendererLayer>(Fmt::Format("{} {}", GetAppInfo().Name, ONU_VERSION_STR), -1, -1, Window::DefaultFlags);
        RegisterLayer<SceneLayer>();

        {
            static PrintMessage object;
            OnEventCallback callback;
            callback.Bind<PrintMessage, &PrintMessage::Print>(&object);
            EventDispatcher::AddListener<KeyPressedEvent>(callback);
        }

        // Editor core windows
        ImGuiLayer* imGui = RegisterLayer<ImGuiLayer>();
        imGui->Register(Memory::New<DockingSpace>());
        imGui->Register(Memory::New<Console>());
        imGui->Register(Memory::New<Hierarchy>());

        FormatterTest(Array({1, 2, 3, 4, 5, 6, 7}), "{cp|rs|rb}");
        FormatterTest(Array({"Bob", "Jeff", "Andre", "Jess", "Syllia"}), "{ln|num|rb}", true);
        FormatterTest(Vector4(1.0f, 2.0f, 3.0f, 4.0f), "{x}");
        FormatterTest(TransformComponent(Vector3(5.0f, 5.0f, 5.0f), Vector3(1.0f), Vector3(0.0f)), "{ln}", true);

        String str(Fmt::Format("This {} {.2} {} test\nTransform component:\n{ln}", String("is"), PI, StringView("a"), TransformComponent(Vector3(5.0f, 5.0f, 5.0f), Vector3(1.0f), Vector3(0.0f))));
        LOG(Warning, str);

        SetupTestScene();
    }
}

namespace Oniun
{
    Engine* CreateApplication(const CommandLineArguments& args)
    {
        return Memory::New<Editor::Application>(args);
    }

    // Add custom formatters
    template <>
    struct Formatter<Editor::TransformComponent>
    {
        bool NewLine = false;

        bool Parse(const FormatArgsContext& context)
        {
            for (StringView arg : context)
            {
                if (arg == "ln")
                    NewLine = true;
            }
            return true;
        }

        void FormatTo(String& dest, const Editor::TransformComponent& transform)
        {
            Formatter<Vector3> fmt;

            dest.Append("Position: ");
            fmt.FormatTo(dest, transform.Position);

            if (NewLine)
                dest.Append("\nScale:    ");
            else
                dest.Append(", Scale: ");
            fmt.FormatTo(dest, transform.Scale);

            if (NewLine)
                dest.Append("\nRotation: ");
            else
                dest.Append(", Rotation: ");
            fmt.FormatTo(dest, transform.Rotation);
        }
    };
}
