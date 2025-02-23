#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32FileSystem.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/Base/IFileSystem.h"
#include "Oniun/Platform/Windows/Win32Headers.h"
#include "Oniun/Platform/Windows/Win32Platform.h"

namespace Onu
{
    String FileSystem::GetCurrentDirectory()
    {
        String buffer(256);
        GetCurrentDirectoryW((DWORD)buffer.Capacity(), buffer.Data());
        buffer.ReCalcLength();
        buffer.CorrectPathSlashes();
        return buffer;
    }

    bool FileSystem::CreateDirectory(const StringView& path)
    {
        DWORD fileAttributes = GetFileAttributesW(*path);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        {
            const DWORD error = GetLastError();
            if (error == ERROR_ACCESS_DENIED)
                return false;

            // Recursively create all sub directories
            uint64 slash = path.FindLast('/');
            if (slash != GlobalVars::NoPos)
            {
                String subPath(Slice(path.begin(), path.begin() + slash));
                if (CreateDirectory(subPath))
                    return true;
            }

            if (!CreateDirectoryW(path.Data(), nullptr))
            {
                LOG(Warning, *Platform::GetLastErrorMessage());
                return false;
            }

            return true;
        }
        return false;
    }

    bool FileSystem::DeleteDirectory(const StringView& path)
    {
        // DWORD fileAttributes = GetFileAttributesW(*path);
        // if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        // {
        //     RemoveDirectoryW(*path);
        // }
        // LOG(Warning, *Platform::GetLastErrorMessage());
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

    bool FileSystem::GetDirectoryFiles(Array<String>& filePaths, const StringView& path,
                                       const StringView& searchPattern, DirectorySearch searchOpt)
    {
        if (searchOpt == DirectorySearch::FilesOnly)
            return GetDirectoryFilesOnly(filePaths, path, searchPattern);
        return GetDirectoryFilesAll(filePaths, path, searchPattern);
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

    bool FileSystem::GetDirectoryFilesOnly(Array<String>& filePaths, const StringView& path,
        const StringView& searchPattern)
    {
        // Combine the path and the pattern into one string and initialize windows search handle
        WIN32_FIND_DATAW findData;
        HANDLE handle = FindFirstFileW(*(path / searchPattern), &findData);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Warning, *Platform::GetLastErrorMessage());
            return false;
        }

        // Iterate over each file and folder and add the files to the array
        do
        {
            StringView name(findData.cFileName);
            if (name == TEXT(".") || name == TEXT(".."))
                continue;

            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                filePaths.Add(path / name);
        }
        while (FindNextFileW(handle, &findData));

        FindClose(handle);
        return true;
    }

    bool FileSystem::GetDirectoryFilesAll(Array<String>& filePaths, const StringView& path,
        const StringView& searchPattern)
    {
        GetDirectoryFilesOnly(filePaths, path, searchPattern);

        // Combine the path and the pattern into one string and initialize windows search handle
        WIN32_FIND_DATAW findData;
        HANDLE handle = FindFirstFileW(*(path + '/'), &findData);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Warning, *Platform::GetLastErrorMessage());
            return false;
        }

        // Search for all directories within and iterate over each one to collect the files
        do
        {
            StringView name(findData.cFileName);
            if (name == TEXT(".") || name == TEXT(".."))
                continue;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!GetDirectoryFilesAll(filePaths, path / name, searchPattern))
                    return false;
            }
        } while (FindNextFileW(handle, &findData));

        FindClose(handle);
        return false;
    }
}

#endif
