#include "Oniun/RHI/Base/IImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Renderer/RendererLayer.h"

IImGuiLayer::IImGuiLayer()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // TODO: Serialize style and colors and then call setup
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
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
