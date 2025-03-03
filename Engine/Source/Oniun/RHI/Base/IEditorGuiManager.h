#pragma once

class IEditorGuiManager
{
public:
    IEditorGuiManager();
    virtual ~IEditorGuiManager()
    {
    }

    virtual void Initialize(class Renderer* renderer);
    virtual void Terminate();
    virtual void NewFrame();
    virtual void RenderPlatformDrawData() = 0;

    void Render();
    void UpdatePlatformWindows();
};
