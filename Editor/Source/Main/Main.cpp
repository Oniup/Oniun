#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Vector3.h"
#include "Oniun/Core/Templates/Function.h"
#include "Oniun/PLatform/EntryPoint.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/Scene/Entity.h"
#include "Oniun/Scene/Scene.h"
#include "Oniun/Scene/SceneLayer.h"

struct TransformComponent
{
    Vector3 Position;
    Vector3 Scale;
    Vector3 Rotation;

    Vector3 GetPosition()
    {
        return Position;
    }
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

int EntryPoint(const CommandLineArguments& args)
{
    Logger logger;
#if !ONU_DIST
    Logger::AddOutput(Memory::New<TerminalLogOutput>());
#endif
    Logger::AddOutput(Memory::New<FileLogOutput>("OutputFile.txt"));

    Engine engine(AppInfo{
        .Name = "Oniun Engine",
        .CommandLineArguments = args,
        .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        .EngineBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    const AppInfo& info = Engine::GetAppInfo();
    Engine::RegisterLayer<RendererLayer>(Format("{} ({})", info.Name, info.EngineBuild), -1, -1, Window::DefaultFlags);

    ImGuiLayer* imGui = Engine::RegisterLayer<ImGuiLayer>();
    imGui->Add(Memory::New<DockingSpace>());
    imGui->Add(Memory::New<Console>());
    imGui->Add(Memory::New<Hierarchy>());

    Engine::RegisterLayer<SceneLayer>();
    {
        Scene scene;
        {
            Entity slimeBase = scene.Add("Slime");
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
            Entity playerBase = scene.Add("Player");
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

            Entity environment = scene.Add("Environment");
            environment.AddChild("Floor");
            addTree(environment);
            addTree(environment);
            addTree(environment);
            addTree(environment);
        }

        FixedArray<StringView, 3> targets({"Player", "Environment", "Slime"});
        for (auto&[name, entry] : scene.GetEntityEntries())
        {
            Entity entity(&name, &entry, &scene);
            for (const StringView& targetName : targets)
            {
                if (entity.GetName() == targetName)
                    LOG(Info, "{}", entity);
            }
        }
    }

    engine.Run();
    return 0;
}
