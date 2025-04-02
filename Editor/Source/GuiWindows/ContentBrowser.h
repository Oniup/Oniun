#pragma once

#include "Oniun/Renderer/IImGuiWindow.h"

namespace Oniun
{
    class ContentBrowser : public IImGuiWindow
    {
    public:
        ContentBrowser();

        void Draw() override;
    };
}
