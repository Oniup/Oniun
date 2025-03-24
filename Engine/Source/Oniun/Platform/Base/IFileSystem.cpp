#include "Oniun/Platform/Base/IFileSystem.h"

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringView.h"
#include "Oniun/Platform/FileSystem.h"

namespace Oniun
{
    String IFileSystem::RelativePathToAbsolute(const StringView& path)
    {
        return FileSystem::GetCurrentDirectory() / path;
    }

    String IFileSystem::AbsolutePathToRelative(const StringView& path)
    {
        String cwd(FileSystem::GetCurrentDirectory());
        uint64 index = path.Find(cwd);

        if (index == NO_POS)
            return path;
        return ToSlice(path, index + cwd.Length() + 1, path.Length() - (index + 1));
    }

    String IFileSystem::GetTemporaryFileName(const StringView& currentPath)
    {
        Slice fileName(GetFileNameFromPath(currentPath));
        return FileSystem::GetSpecialDirectoryPath(SpecialDirectory::Temporary) / fileName;
    }

    Slice<char> IFileSystem::GetFileNameFromPath(const StringView& path)
    {
        uint64 index = path.FindLast('/');
        if (index != NO_POS && path.Last() != '/')
            return ToSlice(path.Begin() + index + 1, path.End());
        return ToSlice(path);
    }

    Slice<char> IFileSystem::GetDirectoryNameFromFilePath(const StringView& path)
    {
        uint64 extIndex = path.FindLast('.');
        uint64 slashIndex = path.FindLast('/');
        if (extIndex != NO_POS && slashIndex != NO_POS && extIndex > slashIndex)
            return ToSlice(path.Begin(), path.Begin() + slashIndex - 1);
        return ToSlice(path);
    }
}
