#pragma once

#include "Oniun/RHI/IImGuiWindow.h"

class Hierarchy : public IImGuiWindow
{
public:
    Hierarchy();

    void Draw() override;
};
