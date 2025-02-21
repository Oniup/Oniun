#include <cwchar>

#include <Oniun/Core/String/String.h>
#include <Oniun/Core/String/StringUtils.h>
#include "Main/Tests.h"

using namespace Onu;

int main()
{
    CharString utf8(StringUtils::Utf16ToUtf8(TEXT("Привет")));
    String utf16(StringUtils::Utf8ToUtf16("Привет"));

    if (utf8 == "Привет")
        std::printf("Yippeeeee");
    if (utf16 == TEXT("Привет"))
        std::printf("Yippeeeee");

    return 0;
}
