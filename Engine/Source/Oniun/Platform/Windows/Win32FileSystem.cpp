#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32FileSystem.h"

#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/Base/IFileSystem.h"
#include "Oniun/Platform/Windows/Win32Headers.h"
#include "Oniun/Platform/Windows/Win32Platform.h"

#include <KnownFolders.h>
#include <ShlObj.h>

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
                LOG(Error, *Platform::GetLastErrorMessage());
                return false;
            }
            return true;
        }
        return false;
    }

    bool FileSystem::DeleteDirectory(const StringView& path)
    {
        DWORD fileAttributes = GetFileAttributesW(*path);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }

        WIN32_FIND_DATA findData;
        HANDLE handle = FindFirstFileW(*(path / '*'), &findData);
        do
        {
            StringView name(findData.cFileName);
            if (name == TEXT(".") || name == TEXT(".."))
                continue;

            String filePath = path / name;
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!DeleteDirectory(*filePath))
                {
                    FindClose(handle);
                    return false;
                }
            }
            else
            {
                if (!DeleteFileW(*filePath))
                {
                    LOG(Error, *Platform::GetLastErrorMessage());
                    FindClose(handle);
                    return false;
                }
            }
        }
        while (FindNextFileW(handle, &findData));
        FindClose(handle);

        if (!RemoveDirectoryW(*path))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }

        return true;
    }

    bool FileSystem::DirectoryExists(const StringView& path)
    {
        DWORD attrib = GetFileAttributesW(*path);
        return attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY);
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
        WIN32_FIND_DATA findData;
        HANDLE handle = FindFirstFileW(*(path / '*'), &findData);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            FindClose(handle);
            return false;
        }

        do
        {
            StringView name(findData.cFileName);
            if (name == TEXT(".") || name == TEXT(".."))
                continue;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                directories.Add(path / name);
        }
        while (FindNextFileW(handle, &findData));

        FindClose(handle);
        return true;
    }

    String FileSystem::GetSpecialDirectoryPath(SpecialDirectory type)
    {
        String result;
        KNOWNFOLDERID folderId;
        switch (type)
        {
        case SpecialDirectory::Desktop: folderId = FOLDERID_Desktop; break;
        case SpecialDirectory::Documents: folderId = FOLDERID_Documents; break;
        case SpecialDirectory::Downloads: folderId = FOLDERID_Downloads; break;
        case SpecialDirectory::Music: folderId = FOLDERID_Music; break;
        case SpecialDirectory::Pictures: folderId = FOLDERID_Pictures; break;
        case SpecialDirectory::Videos: folderId = FOLDERID_Videos; break;
        case SpecialDirectory::AppData: folderId = FOLDERID_RoamingAppData; break;
        case SpecialDirectory::LocalAppData: folderId = FOLDERID_LocalAppData; break;
        case SpecialDirectory::ProgramData: folderId = FOLDERID_ProgramData; break;
        case SpecialDirectory::Temporary:
            {
                Char tempPath[MAX_PATH];
                if (GetTempPathW(MAX_PATH, tempPath))
                {
                    result.Set(tempPath);
                    result.CorrectPathSlashes();
                }
            }
            return result;
        }

        PWSTR tempPath = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(folderId, 0, nullptr, &tempPath)))
        {
            result.Set(tempPath);
            result.CorrectPathSlashes();
        }
        CoTaskMemFree(tempPath);
        return result;
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
            LOG(Error, *Platform::GetLastErrorMessage());
            FindClose(handle);
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
        HANDLE handle = FindFirstFileW(*(path / '*'), &findData);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            FindClose(handle);
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
                {
                    FindClose(handle);
                    return false;
                }
            }
        }
        while (FindNextFileW(handle, &findData));

        FindClose(handle);
        return false;
    }
}

#endif
