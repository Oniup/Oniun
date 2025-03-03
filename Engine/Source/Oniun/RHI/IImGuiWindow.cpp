#include "Oniun/RHI/IImGuiWindow.h"

IImGuiWindow::IImGuiWindow(const StringView& title, ImGuiWindowFlags flags, bool open)
    : m_Title(title), m_Flags(flags), m_Open(open)
{
}

bool IImGuiWindow::Begin()
{
    return ImGui::Begin(*m_Title, &m_Open, m_Flags);
}

void IImGuiWindow::End()
{
    ImGui::End();
}
