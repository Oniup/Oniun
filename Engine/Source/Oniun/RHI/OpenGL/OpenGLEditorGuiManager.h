#pragma once

#include "Oniun/RHI/Base/IEditorGuiManager.h"

class EditorGuiManager : public IEditorGuiManager
{
public:
    void Initialize(class Renderer* renderer) override;
    void Terminate() override;
    void NewFrame() override;
    void RenderPlatformDrawData() override;
};
