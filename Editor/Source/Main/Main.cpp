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
    imGui->Register(Memory::New<DockingSpace>());
    imGui->Register(Memory::New<Console>());
    imGui->Register(Memory::New<Hierarchy>());

    Engine::RegisterLayer<SceneLayer>();
    {
        Scene scene;
        Entity player;
        for (uint64 i = 0; i < 1000; ++i)
        {
            Entity entity;
            if (i == 132)
            {
                entity = scene.Add("Player");
                player = entity;
                player.AddChild("Weapon");
            }
            else
                entity = scene.Add();

            entity.Add<TransformComponent>(Vector3(i), Vector3(1.0f), Vector3(0.0f));
        }
    }

    engine.Run();
    return 0;
}
