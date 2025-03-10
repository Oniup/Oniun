#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Vector3.h"
#include "Oniun/PLatform/EntryPoint.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/Scene/Entity.h"
#include "Oniun/Scene/Scene.h"

struct TransformComponent
{
    Vector3 Position;
    Vector3 Scale;
    Vector3 Rotation;
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

int EntryPoint(const CommandLineArguments& args)
{
    Logger logger;
#if !ONU_DIST
    Logger::AddOutput(Memory::Allocate<TerminalLogOutput>());
#endif
    Logger::AddOutput(Memory::Allocate<FileLogOutput>("OutputFile.txt"));

    Engine engine(AppInfo{
        .Name = "Oniun Engine",
        .CommandLineArguments = args,
        .AppBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        .EngineBuild =  AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
    });

    const AppInfo& info = Engine::GetAppInfo();
    Engine::RegisterLayer<RendererLayer>(Format("{} ({})", info.Name, info.EngineBuild), -1, -1, Window::DefaultFlags);

    ImGuiLayer* imGui = Engine::RegisterLayer<ImGuiLayer>();
    imGui->Add(Memory::Allocate<DockingSpace>());
    imGui->Add(Memory::Allocate<Console>());
    imGui->Add(Memory::Allocate<Hierarchy>());

    {
        Scene scene;
        // Create a ComponentRegistry that supports the 2 components packed into a single array
        scene.AddComponentRegistry<TransformComponent, MeshComponent>();
        Entity entity;

        for (uint64 i = 0; i < 500; ++i)
        {
            // Create an entity and add it the array of entities
            Entity ent = scene.Create();

            // Create transform component, because there isn't any ComponentRegistry for just transform, it would
            // create one and add it
            scene.AddComponent<TransformComponent>(ent);
            // The previous is transform component, it needs to move the transform component out the component registry
            // it currently exists and move it the multi component registry created when the scene was created
            scene.AddComponent<MeshComponent>(ent);

            // Setting the values of the components
            TransformComponent* transform = scene.GetComponent<TransformComponent>(ent);
            MeshComponent* mesh = scene.GetComponent<MeshComponent>(ent);
            transform->Position = Vector3(1.0f, 2.0f, 3.0f);
            mesh->Vertices.Resize(512); // would read from a file and set the vertices and indices
            mesh->Indices.Resize(512);

            if (i == 255)
                entity = ent;
        }

        TransformComponent* sameTransform = entity.GetComponent<TransformComponent>();
        LOG(Info, "Transform Position: {}", sameTransform->Position);
    }

    engine.Run();

    return 0;
}
