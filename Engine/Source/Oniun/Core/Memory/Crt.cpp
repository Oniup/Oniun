#include "Oniun.pch.h"
#include "Oniun/Core/Memory/Crt.h"

#define STB_SPRINTF_IMPLEMENTATION
#include <stb/stb_sprintf.h>

uint64 Crt::Format(char* dest, uint64 destMaxLength, const char* format, va_list args)
{
    return stbsp_vsnprintf(dest, destMaxLength, format, args);
}

uint64 Crt::Format(char* dest, uint64 destMaxLength, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    uint64 written = stbsp_vsnprintf(dest, destMaxLength, format, args);
    va_end(args);
    return written;
}
