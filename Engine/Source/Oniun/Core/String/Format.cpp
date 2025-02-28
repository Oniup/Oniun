#include "Oniun/Core/String/Format.h"

#include <cwchar>

#define PRIMITIVE_TO_STRING_FUNC_IMPL(_Type, _Format)\
    String ToString(_Type val)\
    {\
        constexpr uint64 maxLength = 30;\
        Char buffer[maxLength];\
        std::swprintf(buffer, maxLength, _Format, val);\
        return String(buffer);\
    }


PRIMITIVE_TO_STRING_FUNC_IMPL(int64, TEXT("%lld"))
PRIMITIVE_TO_STRING_FUNC_IMPL(int32, TEXT("%d"))
PRIMITIVE_TO_STRING_FUNC_IMPL(int16, TEXT("%d"))
PRIMITIVE_TO_STRING_FUNC_IMPL(int8, TEXT("%d"))

PRIMITIVE_TO_STRING_FUNC_IMPL(uint64, TEXT("%llu"))
PRIMITIVE_TO_STRING_FUNC_IMPL(uint32, TEXT("%u"))
PRIMITIVE_TO_STRING_FUNC_IMPL(uint16, TEXT("%u"))
PRIMITIVE_TO_STRING_FUNC_IMPL(uint8, TEXT("%u"))

PRIMITIVE_TO_STRING_FUNC_IMPL(float, TEXT("%f"))
PRIMITIVE_TO_STRING_FUNC_IMPL(double, TEXT("%lf"))

String ToString(const CharStringView& str)
{
    return StringUtils::Utf8ToUtf16(str);
}
