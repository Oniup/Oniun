#pragma once

#include <Oniun/Core/Engine.h>
#include <Oniun/Event/Event.h>

namespace Oniun::Editor
{
    class Application : public Engine
    {
    public:
        Application(const CommandLineArguments& args);

        void Setup() override;
    };
}
