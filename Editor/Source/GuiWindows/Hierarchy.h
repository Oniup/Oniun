#pragma once

#include "Oniun/RHI/IImGuiWindow.h"

namespace Oniun
{
    class Hierarchy : public IImGuiWindow
    {
    public:
        Hierarchy();

        void Draw() override;

    };
}
