#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Base/IFileSystem.h"

namespace Onu
{
    struct FileSystem : IFileSystem
    {
        static String GetCurrentWorkingDirectory();

        static bool CreateDirectory(const StringView& path);
        static bool DeleteDirectory(const StringView& path, bool deleteContents = true);
        static bool CopyDirectory(const StringView& destPath, const StringView& srcPath, bool copySubDirectories = true);

        static bool DirectoryExists(const StringView& path);
        static bool GetDirectoryFiles(Array<String>& filePaths, const StringView& path, const StringView& searchPattern = TEXT("*"), DirectorySearch searchOpt = DirectorySearch_All);
        static bool GetChildDirectories(Array<String>& directories, const StringView& path);

        static String OpenSpecialDirectory(SpecialDirectory type);

        static bool DeleteFile(const StringView& path);
        static bool MoveFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting = false);
        static bool CopyFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting = false);

        static uint64 GetFileSize(const StringView& path);
        static bool FileExists(const StringView& path);
    };
}

#endif
