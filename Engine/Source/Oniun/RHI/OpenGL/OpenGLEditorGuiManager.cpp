#include "Oniun/RHI/OpenGL/OpenGLEditorGuiManager.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Oniun/Renderer/Renderer.h"

void EditorGuiManager::Initialize(Renderer* renderer)
{
    IEditorGuiManager::Initialize(renderer);
    ImGui_ImplGlfw_InitForOpenGL(renderer->GetWindow()->GetInternalWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void EditorGuiManager::Terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    IEditorGuiManager::Terminate();
}

void EditorGuiManager::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();

    IEditorGuiManager::NewFrame();
}

void EditorGuiManager::RenderPlatformDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
