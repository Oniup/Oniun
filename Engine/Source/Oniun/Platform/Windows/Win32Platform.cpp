#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32Platform.h"

#include <Windows.h>

#include "Oniun/Core/String/Format.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Platform/File.h"

namespace Onu
{
    String Platform::GetLastErrorMessage()
    {
        uint32 errorCode = GetLastError();
        constexpr uint32 maxLength = 512;
        Char buffer[maxLength];
        DWORD msgSize = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode,
                                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, maxLength, NULL);
        if (msgSize > 0)
            return Format(TEXT("Windows Error:{} => {}"), errorCode, String(buffer, msgSize));
        return Format(TEXT("Windows Error:{} => Unknown"), errorCode);
    }

    File Platform::GetStdOutStream()
    {
        return File(GetStdHandle(STD_OUTPUT_HANDLE), false);
    }

    File Platform::GetStdErrorStream()
    {
        return File(GetStdHandle(STD_ERROR_HANDLE), false);
    }
}

#endif
