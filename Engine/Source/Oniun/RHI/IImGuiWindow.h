#pragma once

#include <imgui/imgui.h>

#include "Oniun/Core/String/String.h"

class IImGuiWindow
{
protected:
    String m_Title;
    ImGuiWindowFlags m_Flags;
    bool m_Open;

public:
    IImGuiWindow(const StringView& title, ImGuiWindowFlags flags = ImGuiWindowFlags_None, bool open = true);

    virtual ~IImGuiWindow()
    {
    }

    FORCE_INLINE const String& GetTitle() const
    {
        return m_Title;
    }

    FORCE_INLINE ImGuiWindowFlags GetFlags() const
    {
        return m_Flags;
    }

    FORCE_INLINE bool IsOpened() const
    {
        return m_Open;
    }

    FORCE_INLINE void SetTitle(const StringView& title)
    {
        m_Title = title;
    }

    FORCE_INLINE void SetFlags(ImGuiWindowFlags flags)
    {
        m_Flags = flags;
    }

    FORCE_INLINE void SetIsOpened(bool open)
    {
        m_Open = open;
    }

    virtual void Draw() = 0;

    virtual bool Begin();
    virtual void End();
};
