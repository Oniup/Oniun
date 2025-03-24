#include "Oniun/Core/String/Format.h"

#include <cstdio>

#define PRIMITIVE_TO_STRING_FUNC_IMPL(_Type, _Format)\
    String ToString(_Type val)\
    {\
        constexpr uint64 maxLength = 30;\
        char buffer[maxLength];\
        std::snprintf(buffer, maxLength, _Format, val);\
        return String(buffer);\
    }


namespace Oniun
{
    PRIMITIVE_TO_STRING_FUNC_IMPL(int64, "%lld")
    PRIMITIVE_TO_STRING_FUNC_IMPL(int32, "%d")
    PRIMITIVE_TO_STRING_FUNC_IMPL(int16, "%d")
    PRIMITIVE_TO_STRING_FUNC_IMPL(int8, "%d")

    PRIMITIVE_TO_STRING_FUNC_IMPL(uint64, "%llu")
    PRIMITIVE_TO_STRING_FUNC_IMPL(uint32, "%u")
    PRIMITIVE_TO_STRING_FUNC_IMPL(uint16, "%u")
    PRIMITIVE_TO_STRING_FUNC_IMPL(uint8, "%u")

    PRIMITIVE_TO_STRING_FUNC_IMPL(float, "%f")
    PRIMITIVE_TO_STRING_FUNC_IMPL(double, "%lf")
}
