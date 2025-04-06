#pragma once

#include "Oniun/RHI/Base/IContext.h"

namespace Oniun::RHI
{
    class Context : public IContext
    {
    public:
        Context();
        ~Context() override;

        void Initialize() override;
        void SetWireframeMode(bool enable) override;
    };
}
