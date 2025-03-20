#pragma once

#include <imgui/imgui.h>

#include "Oniun/Core/String/String.h"

class IImGuiWindow
{
public:
    static constexpr ImGuiWindowFlags DefaultFlags = ImGuiWindowFlags_NoCollapse;

public:
    IImGuiWindow(const StringView& title, ImGuiWindowFlags flags = DefaultFlags, bool destroyOnClose = false, bool open = true);

    virtual ~IImGuiWindow()
    {
    }

public:
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

    FORCE_INLINE bool DestroyOnClose()
    {
        return m_DestroyOnClose;
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

    virtual void Begin();
    virtual void End();

protected:
    String m_Title;
    ImGuiWindowFlags m_Flags;
    bool m_DestroyOnClose;
    bool m_Open;

};
