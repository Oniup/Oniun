#include "Oniun/RHI/OpenGL/OpenGLEditorGuiManager.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Oniun/Renderer/Renderer.h"

void ImGuiWindowManager::Initialize(Renderer& renderer)
{
    IImGuiWindowManager::Initialize(renderer);
    ImGui_ImplGlfw_InitForOpenGL(renderer.GetWindow()->GetInternalWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiWindowManager::Terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    IImGuiWindowManager::Terminate();
}

void ImGuiWindowManager::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();

    IImGuiWindowManager::NewFrame();
}

void ImGuiWindowManager::RenderPlatformDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
