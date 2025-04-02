#pragma once

#include "GuiWindows/Hierarchy.h"
#include "Oniun/Renderer/IImGuiWindow.h"
#include "Oniun/Scene/Entity.h"

namespace Oniun::Editor
{
    class Inspector : public IImGuiWindow
    {
    public:
        Inspector(Hierarchy* hierarchy);

        void Draw() override;

    private:
        Hierarchy* m_Hierarchy;
    };
}
