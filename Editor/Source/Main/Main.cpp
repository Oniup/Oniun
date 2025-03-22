#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include <thread>
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
    imGui->Register(Memory::New<DockingSpace>());
    imGui->Register(Memory::New<Console>());
    imGui->Register(Memory::New<Hierarchy>());

    Engine::RegisterLayer<SceneLayer>();
    std::thread thread([]()
    {
        Scene scene;
        Array<Entity> firstTargets;
        uint64 count = 1;
        for (uint64 i = 0; i < 500; ++i)
        {
            Entity entity = scene.Add(ToString(i));
            if (i % 5 == 0)
            {
                for (uint64 j = 0; j < count; ++j)
                    entity.AddChild();
                ++count;
            }
            if (i % 25 == 0)
                firstTargets.Add(entity);
        }
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

        for (Entity& ent : firstTargets)
            LOG(Info, "{}", ent);

        FixedArray<StringView, 3> targets({"Player", "Environment", "Slime"});
        for (auto& [id, entry] : scene.GetEntityEntries())
        {
            Entity entity(id, &scene);
            for (const StringView& targetName : targets)
            {
                if (entity.GetName() == targetName)
                    LOG(Info, "{}", entity);
            }
        }
    });

    engine.Run();
    thread.join();

    return 0;
}
