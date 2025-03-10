#include "Oniun/RHI/Base/IImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/RHI/ImGuiLayer.h"

static constexpr StringView EngineDefaultFont = "../../../Engine/Assets/Fonts/Lato/Lato-Regular.ttf";
static constexpr uint64 EngineDefaultFontSize = 20;

// void WindowPosCallback(GLFWwindow* window, int xPos, int yPos)
// {
//     static GLFWmonitor* lastMonitor = nullptr;
//     int32 count;
//     GLFWmonitor** monitors = glfwGetMonitors(&count);
//
//     GLFWmonitor* monitor = nullptr;
//     for (int32 i = 0; i < count; ++i)
//     {
//         const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
//         int32 mX, mY;
//         glfwGetMonitorPos(monitors[i], &mX, &mY);
//         if (xPos >= mX && xPos < (mX + mode->width))
//         {
//             if (yPos >= mY && yPos < (mY + mode->height))
//             {
//                 monitor = monitors[i];
//                 break;
//             }
//         }
//     }
//
//     if (monitor != lastMonitor)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.FontGlobalScale =  1.0f / io.FontDefault->FontSize;
//         lastMonitor = monitor;
//     }
// }

IImGuiLayer::IImGuiLayer()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // TODO: Serialize style and colors and then call setup
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    ImGui::GetIO().Fonts->AddFontFromFileTTF(*EngineDefaultFont, EngineDefaultFontSize);
}

IImGuiLayer::~IImGuiLayer()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool IImGuiLayer::Add(IImGuiWindow* window)
{
    if (window)
    {
        if (!window->GetTitle().IsEmpty())
        {
            m_Windows.Add(window);
            return true;
        }
        Memory::Free(window);
    }
    return false;
}

void IImGuiLayer::OnStart()
{
    RendererLayer* renderer = Engine::GetLayer<RendererLayer>();
    renderer->SetImGuiWindowLayer(static_cast<ImGuiLayer*>(this));
}

void IImGuiLayer::NewFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void IImGuiLayer::Render(RendererLayer& renderer)
{
    NewFrame();

    for (IImGuiWindow* window : m_Windows)
    {
        if (window->Begin())
        {
            window->Draw();
            window->End();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)renderer.GetWindow()->GetWidth(), (float)renderer.GetWindow()->GetHeight());

    ImGui::Render();
}

void IImGuiLayer::UpdatePlatformWindows()
{
    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupCurrentContext);
}

void IImGuiLayer::SetFont(const StringView& font, uint64 fontSize)
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    if (!io.Fonts->AddFontFromFileTTF(*font, fontSize))
        io.Fonts->AddFontFromFileTTF(*EngineDefaultFont, EngineDefaultFontSize);
}
