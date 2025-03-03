#include "Oniun/RHI/Base/IEditorGuiManager.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Renderer/Renderer.h"

IEditorGuiManager::IEditorGuiManager()
{
}

void IEditorGuiManager::Initialize(Renderer* renderer)
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

void IEditorGuiManager::Terminate()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IEditorGuiManager::NewFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void IEditorGuiManager::Render()
{
    ImGui::Render();
}

void IEditorGuiManager::UpdatePlatformWindows()
{
    GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backupCurrentContext);
}
