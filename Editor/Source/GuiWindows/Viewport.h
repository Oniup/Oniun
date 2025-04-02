#pragma once

#include "Oniun/Renderer/IImGuiWindow.h"

namespace Oniun::Editor
{
    class Viewport : public IImGuiWindow
    {
    public:
        Viewport();

        void Draw() override;

    private:
    };
}
