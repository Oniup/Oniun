#pragma once

#include "Oniun/Core/ClassConstraints.h"

namespace Onu
{
    class Engine : public Singleton<Engine>
    {
    public:
        Engine();
        ~Engine();
    };
}
