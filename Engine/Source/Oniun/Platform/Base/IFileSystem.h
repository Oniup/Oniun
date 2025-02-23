#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/String.h"

namespace Onu
{
    enum class DirectorySearch
    {
        /// Includes current directory and all subdirectories in the search.
        All,
        /// Includes only the current directory.
        FilesOnly,
    };

    enum class SpecialDirectory
    {
        Desktop,
        Documents,
        Downloads,
        Music,
        Pictures,
        Videos,
        AppData,
        LocalAppData,
        ProgramData,
        Temporary,
    };

    struct IFileSystem
    {
        static String GetCurrentDirectory() = delete;

        static bool CreateDirectory(const StringView& path) = delete;
        static bool DeleteDirectory(const StringView& path) = delete;

        static bool DirectoryExists(const StringView& path) = delete;
        static bool GetDirectoryFiles(Array<String>& filePaths, const StringView& path,
                                      const StringView& searchPattern = TEXT("*"),
                                      DirectorySearch searchOpt = DirectorySearch::All) = delete;
        static bool GetChildDirectories(Array<String>& directories, const StringView& path) = delete;

        static String GetSpecialDirectoryPath(SpecialDirectory type) = delete;

        static bool DeleteFile(const StringView& path) = delete;
        static bool MoveFile(const StringView& destPath, const StringView& srcPath,
                             bool overwriteExisting = false) = delete;
        static bool CopyFile(const StringView& destPath, const StringView& srcPath,
                             bool overwriteExisting = false) = delete;

        static uint64 GetFileSize(const StringView& path) = delete;
        static bool FileExists(const StringView& path) = delete;

        static String RelativePathToAbsolute(const StringView& path);
        static String AbsolutePathToRelative(const StringView& path);
    };
}
