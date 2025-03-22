#include "Oniun.pch.h"
#include "Oniun/RHI/IImGuiWindow.h"

namespace Oniun
{
    IImGuiWindow::IImGuiWindow(const StringView& title, ImGuiWindowFlags flags, bool destroyOnClose, bool open)
        : m_Title(title), m_Flags(flags), m_DestroyOnClose(destroyOnClose), m_Open(open)
    {
    }

    void IImGuiWindow::Begin()
    {
        ImGui::Begin(*m_Title, &m_Open, m_Flags);
    }

    void IImGuiWindow::End()
    {
        ImGui::End();
    }
}
