#include "Oniun.pch.h"
#include "Oniun/RHI/OpenGL/OpenGLImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Renderer/RendererLayer.h"

ImGuiLayer::ImGuiLayer()
{
    RendererLayer* renderer = Engine::GetLayer<RendererLayer>();
    ImGui_ImplGlfw_InitForOpenGL(renderer->GetWindow()->GetInternalWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
}

void ImGuiLayer::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();

    IImGuiLayer::NewFrame();
}

void ImGuiLayer::RenderPlatformDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
