#include <chrono>

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

void TestHashMap();

bool ShouldIncrease(uint64 count, uint64 capacity)
{
    return count * 4 > capacity * 3;
}

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

    TestHashMap();

    Engine::RegisterLayer<SceneLayer>();
    {
        Scene scene;
        scene.AddComponentRegistry<TransformComponent, MeshComponent>();
        scene.AddComponentRegistry<TransformComponent, MeshComponent, RigidbodyComponent, BoxColliderComponent>();

        Entity entity;
        uint64 called = 0;
        for (uint64 i = 0; i < 500; ++i)
        {
            Entity ent = scene.Create();
            if (i == 69)
                entity = ent;
            else if (entity.IsAlive())
            {
                TransformComponent* transform = entity.GetComponent<TransformComponent>();
                ++called;
                transform->Position.X += 1.0f;
            }

            ent.AddComponent<TransformComponent>(Vector3(5.0f, 5.0f, 5.0f));
            ent.AddComponent<MeshComponent>();
            ent.AddComponent<RigidbodyComponent>();
            ent.AddComponent<BoxColliderComponent>();

            MeshComponent* mesh = ent.GetComponent<MeshComponent>();
            mesh->Vertices.Resize(255);
            mesh->Indices.Resize(255);
        }

        TransformComponent* transform = entity.GetComponent<TransformComponent>();
        LOG(Info, "Called: {}, Transform position: {}", called, transform->Position);
    }

    engine.Run();

    return 0;
}

void TestHashMap()
{
    // Default Constructor
    {
        HashMap<StringView, uint64> map;
        ASSERT(map.Count() == 0);
        ASSERT(map.Capacity() == DEFAULT_HASH_MAP_CAPACITY_COUNT);
    }

    {
        HashMap<StringView, uint64> map;
        // Add
        {
            map.Add("Test Key", 1);
            map.Add("Another Key", 22);
            map.Add("Last Key", 40);
            ASSERT(map.Count() == 3);
            ASSERT(map.Capacity() == DEFAULT_HASH_MAP_CAPACITY_COUNT);
            ASSERT(map.At("Test Key") == 1);
            ASSERT(map.At("Last Key") == 40);
            ASSERT(map.At("Another Key") == 22);
        }

        // Iterator
        {
            for (auto& [key, value] : map)
            {
                ASSERT(!key.IsEmpty());
                ASSERT(map.Contains(key));
                ASSERT(map.At(key) == value);
                LOG(Warning, "Iter Key: '{}', Value: {}", key, value);
            }
        }

        // Contains
        {
            ASSERT(map.Contains("Test Key"));
            ASSERT(map.Contains("Another Key"));
            ASSERT(map.Contains("Last Key"));
            ASSERT(!map.Contains("Nonexistent Key"));
        }

        // Clear
        {
            map.Clear();
            ASSERT(map.Count() == 0);
            ASSERT(map.Capacity() == DEFAULT_HASH_MAP_CAPACITY_COUNT);
        }
    }

    // Remove
    {
        HashMap<StringView, uint64> map;
        map.Add("Test Key", 1);
        map.Add("Another Key", 22);
        map.Add("Last Key", 40);
        ASSERT(map.At("Test Key") == 1);
        ASSERT(map.At("Last Key") == 40);
        ASSERT(map.At("Another Key") == 22);
        map.Remove("Another Key");
        ASSERT(!map.Contains("Another Key"));
    }

    // Test with more than 1000 elements
    {
        HashMap<uint64, uint64> map;
        constexpr int numElements = 1000;

        for (uint64 i = 0; i < numElements; ++i)
            map.Add(i, i);

        ASSERT(map.Count() == numElements);
        for (int i = 0; i < numElements; ++i)
        {
            ASSERT(map.Contains(i));
            ASSERT(map.At(i) == i);
        }
        map.Clear();
        ASSERT(map.Count() == 0);
    }
}
