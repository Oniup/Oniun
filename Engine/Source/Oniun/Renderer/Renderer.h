#pragma once

#include "Oniun/Core/EngineLayer.h"
#include "Oniun/Renderer/Window.h"
#include "Oniun/RHI/EditorGuiManger.h"

class Renderer : public EngineLayer
{
    API_ENGINE_LAYER()

private:
    Window m_Window;
    EditorGuiManager m_EditorGui;

public:
    Renderer(const CharStringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags = Window::DefaultFlags);
    ~Renderer() override;

    FORCE_INLINE Window* GetWindow()
    {
        return &m_Window;
    }

    FORCE_INLINE const Window* GetWindow() const
    {
        return &m_Window;
    }

    FORCE_INLINE EditorGuiManager* GetEditorGuiManager()
    {
        return &m_EditorGui;
    }

    FORCE_INLINE const EditorGuiManager* GetEditorGuiManager() const
    {
        return &m_EditorGui;
    }

    void OnStart() override;
    void OnUpdate() override;
};
