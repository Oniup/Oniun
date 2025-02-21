#include "Oniun/Platform/Base/IFileSystem.h"

#include <cwchar>
#include <direct.h>

namespace Onu
{
    String IFileSystem::RelativePathToAbsolute(const StringView& path)
    {
        return String::Empty;
    }

    String IFileSystem::AbsolutePathToRelative(const StringView& path)
    {
        return String::Empty;
    }
}
