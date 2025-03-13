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

class MyClass
{
public:
    String PrintMessage(const StringView& message)
    {
        return Format("Message: {}", message);
    }
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
        .EngineBuild =  AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    const AppInfo& info = Engine::GetAppInfo();
    Engine::RegisterLayer<RendererLayer>(Format("{} ({})", info.Name, info.EngineBuild), -1, -1, Window::DefaultFlags);

    ImGuiLayer* imGui = Engine::RegisterLayer<ImGuiLayer>();
    imGui->Add(Memory::New<DockingSpace>());
    imGui->Add(Memory::New<Console>());
    imGui->Add(Memory::New<Hierarchy>());

    Engine::RegisterLayer<SceneLayer>();

    MyClass myObject;
    Function<String(const StringView&)> func;
    func.Bind<MyClass, &MyClass::PrintMessage>(&myObject);
    LOG(Info, "Function call result: {}", func("This is a test"));

    // {
    //     Scene scene;
    //     scene.AddComponentRegistry<TransformComponent, MeshComponent>();
    //     scene.AddComponentRegistry<TransformComponent, MeshComponent, RigidbodyComponent, BoxColliderComponent>();
    //
    //     Entity entity;
    //     uint64 called = 0;
    //     for (uint64 i = 0; i < 5000; ++i)
    //     {
    //         Entity ent = scene.Create();
    //         if (i == 69)
    //             entity = ent;
    //         if (entity.IsAlive())
    //         {
    //             TransformComponent* transform = entity.GetComponent<TransformComponent>();
    //             ++called;
    //             transform->Position.X += 1.0f;
    //             if (i % 100 == 0)
    //                 LOG(Info, "Position at {}: {}", called, transform->Position);
    //         }
    //
    //         ent.AddComponent<TransformComponent>();
    //         ent.AddComponent<MeshComponent>();
    //         ent.AddComponent<RigidbodyComponent>();
    //         ent.AddComponent<BoxColliderComponent>();
    //
    //         MeshComponent* mesh = ent.GetComponent<MeshComponent>();
    //         mesh->Vertices.Resize(255);
    //         mesh->Indices.Resize(255);
    //     }
    //
    //     TransformComponent* transform = entity.GetComponent<TransformComponent>();
    //     LOG(Info, "Called: {}, Transform position: {}", called, transform->Position);
    // }

    engine.Run();

    return 0;
}
