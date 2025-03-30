#include "Oniun/RHI/Base/IImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Renderer/RendererLayer.h"
#include "Oniun/RHI/ImGuiLayer.h"
#include "Oniun/Event/Event.h"
#include "Oniun/Event/WindowEvents.h"

namespace Oniun
{
    static constexpr StringView EngineDefaultFont = ONIUN_RESOURCE_DIRECTORY "/Assets/Fonts/Arimo/ArimoNerdFont-Regular.ttf";
    static constexpr StringView EngineMonoFont = ONIUN_RESOURCE_DIRECTORY "/Assets/Fonts/Hack/HackNerdFontMono-Regular.ttf";
    static constexpr uint64 EngineDefaultFontSize = 20;

    static void CorrectScaleBasedOnMonitorContentScaleCallback(IEvent* event, void* sender)
    {
        static float lastXScale = 1;
        static float lastYScale = 1;

        WindowSetPositionEvent* setPosEvent = static_cast<WindowSetPositionEvent*>(event);
        GLFWwindow* window = setPosEvent->GetWindow()->GetInternalWindow();

        float xScale, yScale;
        glfwGetWindowContentScale(window, &xScale, &yScale);
        if (xScale != lastXScale || yScale != lastYScale)
        {
            int32 width, height;
            glfwGetWindowSize(window, &width, &height);

            int32 newWidth = (int32)(width * xScale / lastXScale);
            int32 newHeight = (int32)(height * yScale / lastYScale);
            glfwSetWindowSize(window, newWidth, newHeight);

            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = (xScale + yScale) / 2.0f;

            lastXScale = xScale;
            lastYScale = yScale;
        }
    }

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

        ImFontConfig config;
        config.GlyphRanges = GetGlyphRangesRequired();
        m_DefaultFont = io.Fonts->AddFontFromFileTTF(*EngineDefaultFont, EngineDefaultFontSize, &config);
        m_MonoFont = io.Fonts->AddFontFromFileTTF(*EngineMonoFont, EngineDefaultFontSize - 2, &config);

        EventDispatcher::AddListener<WindowSetPositionEvent>(CorrectScaleBasedOnMonitorContentScaleCallback);
        SetTheme();
    }

    IImGuiLayer::~IImGuiLayer()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    const ImWchar* IImGuiLayer::GetGlyphRangesRequired()
    {
        static constexpr ImWchar Range[] = {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x2190, 0x21FF, // Arrows
            0,
        };
        return &Range[0];
    }

    bool IImGuiLayer::Register(IImGuiWindow* window)
    {
        if (window)
        {
            if (!window->GetTitle().IsEmpty())
            {
                m_Windows.Add(window);
                return true;
            }
            Memory::Delete(window);
        }
        return false;
    }

    void IImGuiLayer::OnStart()
    {
        RendererLayer* renderer = Engine::GetLayer<RendererLayer>();
        renderer->SetImGuiWindowLayer(static_cast<ImGuiLayer*>(this));
    }

    void IImGuiLayer::SetTheme()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // General window settings
        style.WindowRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.TabRounding = 5.0f;
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.PopupRounding = 5.0f;

        // Setting the colors
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
        colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

        // Accent colors changed to darker olive-green/grey shades
        colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);        // Dark gray for check marks
        colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);       // Dark gray for sliders
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Slightly lighter gray when active
        colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);           // Button background (dark gray)
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);    // Button hover state
        colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Button active state
        colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);           // Dark gray for menu headers
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);    // Slightly lighter on hover
        colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);     // Lighter gray when active
        colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // Separators in dark gray
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Resize grips in dark gray
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);        // Tabs background
        colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker gray on hover
        colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Docking preview in gray
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Empty dock background
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
            if (window->IsOpened())
            {
                window->Begin();
                window->Draw();
                window->End();
            }
            if (window->DestroyOnClose() && !window->IsOpened())
                m_Windows.Remove(window);
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
}
