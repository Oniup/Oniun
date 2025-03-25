#include "Core/Application.h"

// Main function that calls CreateApplication
#include <Oniun/Core/EntryPoint.h>

// Core engine layers
#include <Oniun/Renderer/RendererLayer.h>
#include <Oniun/RHI/ImGuiLayer.h>
#include <Oniun/Scene/SceneLayer.h>

// Test
#include <Oniun/Core/Math/Vector3.h>
#include <Oniun/PLatform/Thread.h>
#include <Oniun/Scene/ComponentQuery.h>
#include <Oniun/Scene/Entity.h>

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

    struct MessageComponent
    {
        String Message;
    };

    struct MeshComponent
    {
        struct Vertex
        {
            Vector3 Position;
            Vector3 Normal;
            float UvX;
            float UvY;
        };

        Array<Vertex> Vertices;
        Array<uint32> Indices;
    };

    struct RigidbodyComponent
    {
        Vector3 Velocity;
    };

    struct BoxColliderComponent
    {
        struct Face
        {
            Vector3 BottomLeft;
            Vector3 BottomRight;
            Vector3 TopLeft;
            Vector3 TopRight;
        };

        Face LeftFace;
        Face RightFace;
    };

    void SetupTestScene(Scene* scene)
    {
        scene->SetTitle("Test Scene");
        {
            Entity slimeBase = scene->Add("Slime");
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
            playerBase.AddChild("Head");
            Entity torso = playerBase.AddChild("Torso");
            torso.AddChild("Left Arm").AddChild("Hand");
            torso.AddChild("Right Arm").AddChild("Hand");

            Entity hips = torso.AddChild("Hips");
            hips.AddChild("Left Leg").AddChild("Foot");
            hips.AddChild("Right Leg").AddChild("Foot");
        }
        {
            auto addTree = [](Entity& parent)
            {
                Entity tree = parent.AddChild("Tree");
                tree.AddChild("Trunk").AddChild("Leaves");
            };

            Entity environment = scene->Add("Environment");
            environment.AddChild("Floor");
            addTree(environment);
            addTree(environment);
            addTree(environment);
            addTree(environment);
        }

        // FixedArray<StringView, 3> targets({"Player", "Environment", "Slime"});
        // for (auto& [id, entry] : scene->GetEntityEntries())
        // {
        //     Entity entity(id, scene);
        //     for (const StringView& targetName : targets)
        //     {
        //         if (entity.GetName() == targetName)
        //             LOG(Info, "{}", entity);
        //     }
        // }
    }

    Application::Application(const CommandLineArguments& args)
    {
        RegisterAppInfo(AppInfo{
            .Name = "Oniun Engine",
            .CommandLineArguments = args,
            .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
            .EngineBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        });
    }

    void Application::Setup()
    {
        // Setup debug logger
#ifndef NDEBUG
        Logger::AddOutput(Memory::New<TerminalLogOutput>());
#endif
        Logger::AddOutput(Memory::New<FileLogOutput>("OutputFile.txt"));

        // Core engine layers
        RegisterLayer<RendererLayer>(Format("{} {}", GetAppInfo().Name, ONU_VERSION_STR), -1, -1, Window::DefaultFlags);
        SceneLayer* sceneLayer = RegisterLayer<SceneLayer>();

        // Editor core windows
        ImGuiLayer* imGui = RegisterLayer<ImGuiLayer>();
        imGui->Register(Memory::New<DockingSpace>());
        imGui->Register(Memory::New<Console>());
        imGui->Register(Memory::New<Hierarchy>());

        SetupTestScene(sceneLayer->LoadScene());
    }
}

namespace Oniun
{
    Engine* CreateApplication(const CommandLineArguments& args)
    {
        return Memory::New<Editor::Application>(args);
    }
}
