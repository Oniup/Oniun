#pragma once

#include "Oniun/RHI/IImGuiWindow.h"

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
