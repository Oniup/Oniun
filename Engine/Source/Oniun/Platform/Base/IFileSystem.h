#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/String/String.h"

namespace Onu
{
    struct IFileSystem
    {
        enum DirectorySearch
        {
            /// Includes current directory and all subdirectories in the search.
            DirectorySearch_All,
            /// Includes only the current directory.
            TopOnly,
        };

        enum SpecialDirectory
        {
            SpecialDirectory_Desktop,
            SpecialDirectory_Documents,
            SpecialDirectory_Downloads,
            SpecialDirectory_Music,
            SpecialDirectory_Pictures,
            SpecialDirectory_Videos,
            SpecialDirectory_AppData,
            SpecialDirectory_LocalAppData,
            SpecialDirectory_ProgramData,
            SpecialDirectory_Temporary,
        };

        static String GetCurrentWorkingDirectory() = delete;

        static bool CreateDirectory(const StringView& path) = delete;
        static bool DeleteDirectory(const StringView& path, bool deleteContents = true) = delete;
        static bool CopyDirectory(const StringView& destPath, const StringView& srcPath,
                                  bool copySubDirectories = true) = delete;

        static bool DirectoryExists(const StringView& path) = delete;
        static bool GetDirectoryFiles(Array<String>& filePaths, const StringView& path,
                                      const StringView& searchPattern = TEXT("*"),
                                      DirectorySearch searchOpt = DirectorySearch_All) = delete;
        static bool GetChildDirectories(Array<String>& directories, const StringView& path) = delete;

        static String OpenSpecialDirectory(SpecialDirectory type) = delete;

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
