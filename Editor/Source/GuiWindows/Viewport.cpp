#include "GuiWindows/Viewport.h"

namespace Oniun::Editor
{
    Viewport::Viewport()
        : IImGuiWindow("Viewport")
    {
    }

    void Viewport::Draw()
    {
        ImGui::Text("Viewport will be rendered here");
    }
}
