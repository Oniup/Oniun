#include "Win32Platform.h"
#if ONU_PLATFORM_WINDOWS

#include <Windows.h>

#include "Oniun/Platform/Windows/Win32FileSystem.h"
#include "Oniun/Platform/Base/IFileSystem.h"

namespace Onu
{
    String FileSystem::GetCurrentWorkingDirectory()
    {
        String buffer(256);
        ::GetCurrentDirectoryW((DWORD)buffer.Capacity(), buffer.Data());
        buffer.ReCalcLength();
        return buffer;
    }

    bool FileSystem::CreateDirectory(const StringView& path)
    {
        const DWORD fileAttributes = GetFileAttributesW(*path);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        {
        }
        return false;
    }

    bool FileSystem::DeleteDirectory(const StringView& path, bool deleteContents)
    {
        return false;
    }

    bool FileSystem::CopyDirectory(const StringView& destPath, const StringView& srcPath, bool copySubDirectories)
    {
        return false;
    }

    bool FileSystem::DirectoryExists(const StringView& path)
    {
        return false;
    }

    bool FileSystem::GetDirectoryFiles(Array<String>& filePaths, const StringView& path, const StringView& searchPattern, DirectorySearch searchOpt)
    {
        return false;
    }

    bool FileSystem::GetChildDirectories(Array<String>& directories, const StringView& path)
    {
        return false;
    }

    String FileSystem::OpenSpecialDirectory(SpecialDirectory type)
    {
        return String::Empty;
    }

    bool FileSystem::DeleteFile(const StringView& path)
    {
        return false;
    }

    bool FileSystem::MoveFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting)
    {
        return false;
    }

    bool FileSystem::CopyFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting)
    {
        return false;
    }

    uint64 FileSystem::GetFileSize(const StringView& path)
    {
        return 0;
    }

    bool FileSystem::FileExists(const StringView& path)
    {
        return false;
    }
}

#endif
