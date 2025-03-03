#pragma once

#include "Oniun/RHI/Base/IImGuiWindowManager.h"

class Renderer;

class ImGuiWindowManager : public IImGuiWindowManager
{
public:
    void Initialize(Renderer& renderer) override;
    void Terminate() override;
    void NewFrame() override;
    void RenderPlatformDrawData() override;
};
