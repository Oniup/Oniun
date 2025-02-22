#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Platform/Base/IPlatform.h"

namespace Onu
{
    struct Platform : IPlatform
    {
        static String GetLastErrorMessage();

        static File GetStdOutStream();
        static File GetStdErrorStream();
    };
}

#endif
