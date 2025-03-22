#include "Oniun.pch.h"
#include "Oniun/Core/Defines.h"

#include <cstdio>
#include <cstdlib>

namespace Oniun
{
    namespace Assert_Internal
    {
        void Assert(const char* expression, const char* file, const char* function, int line)
        {
            std::fprintf(stderr, "Fatal: (%s) At %s:%d in %s", expression, function, line, file);
            std::exit(-1);
        }
    }
}
