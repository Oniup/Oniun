#pragma once

#include "Oniun/Renderer/IImGuiWindow.h"

namespace Oniun
{
    class DockingSpace : public IImGuiWindow
    {
    public:
        DockingSpace();

        void Draw() override;
        void Begin() override;
    };
}
