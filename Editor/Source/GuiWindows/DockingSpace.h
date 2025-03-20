#pragma once

#include "Oniun/RHI/IImGuiWindow.h"

class DockingSpace : public IImGuiWindow
{
public:
    DockingSpace();

    void Draw() override;
    void Begin() override;
};
