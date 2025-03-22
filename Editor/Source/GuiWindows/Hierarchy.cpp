#include "GuiWindows/Hierarchy.h"

namespace Oniun
{
    Hierarchy::Hierarchy()
        : IImGuiWindow("Hierarchy")
    {
    }

    void Hierarchy::Draw()
    {
        ImGui::TextWrapped("Hello world, this is going to eventually be the Hierarchy window");
    }
}
