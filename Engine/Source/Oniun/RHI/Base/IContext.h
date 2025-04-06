#pragma once

namespace Oniun::RHI
{
    class IContext
    {
    public:
        virtual ~IContext()
        {
        }

        virtual void Initialize() = 0;
        virtual void SetWireframeMode(bool enable) = 0;
    };
}
