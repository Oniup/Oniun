#pragma once

#include "Oniun/Core/String/String.h"

class IEditorGuiWindow
{
    String m_Title;

public:
    IEditorGuiWindow(const StringView& title);

    virtual ~IEditorGuiWindow()
    {
    }
};
