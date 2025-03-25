#include "GuiWindows/DockingSpace.h"

#include <Oniun/Core/Engine.h>
#include <Oniun/Core/Logger.h>
#include <Oniun/RHI/ImGuiLayer.h>

namespace Oniun
{
    DockingSpace::DockingSpace()
        : IImGuiWindow("Docking Space",
                           ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                           ImGuiWindowFlags_NoBackground)
    {
    }

    void DockingSpace::Draw()
    {
        bool printMessage = false;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::MenuItem("Print test message", nullptr, &printMessage);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                Array<IImGuiWindow*>& windows = Engine::GetLayer<ImGuiLayer>()->GetWindows();
                for (IImGuiWindow* window : windows)
                {
                    if (window == this || window->DestroyOnClose())
                        continue;

                    bool open = window->IsOpened();
                    ImGui::MenuItem(*window->GetTitle(), nullptr, &open);
                    window->SetIsOpened(open);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (printMessage)
            LOG(Warning, "This is a test");
    }

    void DockingSpace::Begin()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(*m_Title, &m_Open, m_Flags);
        ImGui::PopStyleVar(3);

        if (m_Open)
        {
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockSpaceId = ImGui::GetID("EngineDockingSpace");
                ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
            }
        }
    }
}
