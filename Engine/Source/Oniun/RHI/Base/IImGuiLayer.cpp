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
