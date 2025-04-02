#pragma once

#include <Oniun/Core/Engine.h>

namespace Oniun::Editor
{
    class Application : public Engine
    {
    public:
        Application(const CommandLineArguments& args);

        void Setup() override;

    private:
        void SetupCoreLayers();
        void SetupGuiWindows();
    };
}
