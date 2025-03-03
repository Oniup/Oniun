#pragma once

#include "Oniun/RHI/IImGuiWindow.h"

class DockingSpace : public IImGuiWindow
{
    bool m_OptPadding;

public:
    DockingSpace();

    void Draw() override;
    bool Begin() override;
};
