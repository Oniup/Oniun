#include "Oniun/Core/Defines.h"

#include <cstdio>
#include <cstdlib>

namespace Oniun
{
    PlatformType GetPlatformType()
    {
#ifdef ONU_PLATFORM_WINDOWS
        return PlatformType::Windows;
#elif ONU_PLATFORM_LINUX
        return PlatformType::Linux;
#elif ONU_PLATFORM_MACOS
        return PlatformType::Macos;
#elif ONU_PLATFORM_XBOX_ONE
        return PlatformType::XboxOne;
#elif ONU_PLATFORM_XBOX_SCARLET
        return PlatformType::XboxScarlet;
#elif ONU_PLATFORM_PS4
        return PlatformType::Ps4;
#elif ONU_PLATFORM_PS5
        return PlatformType::Ps5;
#elif ONU_PLATFORM_SWITCH
        return PlatformType::Switch;
#elif ONU_PLATFORM_IOS
        return PlatformType::Ios;
#elif ONU_PLATFORM_ANDROID
        return PlatformType::Android;
#endif
    }

    namespace Assert_Internal
    {
        void Assert(const char* expression, const char* file, const char* function, int line)
        {
            std::fprintf(stderr, "Fatal: (%s) At %s:%d in %s", expression, function, line, file);
            std::exit(-1);
        }
    }
}

