#include "GuiWindows/Console.h"
#include "GuiWindows/DockingSpace.h"
#include "GuiWindows/Hierarchy.h"

#include <chrono>
#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Math/Vector3.h"
#include "Oniun/Core/Templates/Function.h"
#include "Oniun/PLatform/EntryPoint.h"
#include "Oniun/Platform/FileSystem.h"
#include "Oniun/Platform/Thread.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/Scene/ComponentQuery.h"
#include "Oniun/Scene/Entity.h"
#include "Oniun/Scene/Scene.h"
#include "Oniun/Scene/SceneLayer.h"

namespace Oniun
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

    class TestSceneJob : public IThreadJob
    {
    public:
        TestSceneJob()
            : IThreadJob("Testing Scene")
        {
        }

        int32 Run() override
        {
            auto start = std::chrono::high_resolution_clock::now();
            Scene scene;
            for (uint64 i = 0; i < 1000; ++i)
            {
                Entity entity = scene.Add();
                entity.Add<TransformComponent>(Vector3(i));
                if (i % 2 == 0)
                    entity.Add<MessageComponent>(ToString(i));
            }
            auto end = std::chrono::high_resolution_clock::now();
            float creationDuration = std::chrono::duration<float>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            uint64 i = 0;
            for (ComponentQuery<TransformComponent, MessageComponent> query(scene); !query.IsEnd(); ++query)
            {
                TransformComponent* transform = query.Get<TransformComponent>();
                MessageComponent* message = query.Get<MessageComponent>();
                transform->Position = Vector3(i * 10);
                ++i;
            }
            end = std::chrono::high_resolution_clock::now();
            LOG(Warning, "Creation: {}, Query: {}", creationDuration, std::chrono::duration<float>(end - start).count());
            return 0;
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
            .EngineBuild = AppInfo::Version(ONU_VERSION_MAJOR, ONU_VERSION_MINOR, ONU_VERSION_PATCH),
        });

        const AppInfo& info = Engine::GetAppInfo();
        Engine::RegisterLayer<RendererLayer>(Format("{} {}", info.Name, ONU_VERSION_STR), -1, -1, Window::DefaultFlags);

        LOG(Info, "Current working directory: {}", FileSystem::GetCurrentDirectory());

        ImGuiLayer* imGui = Engine::RegisterLayer<ImGuiLayer>();
        imGui->Register(Memory::New<DockingSpace>());
        imGui->Register(Memory::New<Console>());
        imGui->Register(Memory::New<Hierarchy>());

        Engine::RegisterLayer<SceneLayer>();
        // Thread thread(Memory::New<TestSceneJob>());
        // thread.Start();
        {
            Scene scene;
            for (uint64 i = 0; i < 10; ++i)
            {
                Entity entity = scene.Add();
                entity.Add<TransformComponent>(Vector3(i));
                if (i % 2 == 0)
                    entity.Add<MessageComponent>(ToString(i));
            }

            uint64 i = 0;
            for (ComponentQuery<TransformComponent, MessageComponent> query(scene); !query.IsEnd(); ++query)
            {
                TransformComponent* transform = query.Get<TransformComponent>();
                MessageComponent* message = query.Get<MessageComponent>();
                transform->Position *= 10;
                message->Message.Set("Testing");
                ++i;
            }
            for (ComponentQuery<TransformComponent> query(scene); !query.IsEnd(); ++query)
                LOG(Info, "{}: {}", query.GetCurrentEntityFullName(), query.Get<TransformComponent>()->Position);
        }

        engine.Run();
        // thread.Join();

        return 0;
    }
}
