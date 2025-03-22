#pragma once

#include "Oniun/RHI/Base/IImGuiLayer.h"

namespace Oniun
{
    class RendererLayer;

    class ImGuiLayer : public IImGuiLayer
    {
        API_ENGINE_LAYER()

    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void NewFrame() override;
        void RenderPlatformDrawData() override;
    };
}
