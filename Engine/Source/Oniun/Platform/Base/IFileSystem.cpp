#include "Oniun/Platform/Base/IFileSystem.h"

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Platform/FileSystem.h"

namespace Onu
{
    String IFileSystem::RelativePathToAbsolute(const StringView& path)
    {
        return FileSystem::GetCurrentDirectory() / path;
    }

    String IFileSystem::AbsolutePathToRelative(const StringView& path)
    {
        String cwd(FileSystem::GetCurrentDirectory());
        uint64 index = path.Find(cwd);

        if (index == GlobalVars::NoPos)
            return path;
        return ToSlice(path, index + cwd.Length() + 1, path.Length() - (index + 1));
    }
}
