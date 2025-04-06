#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32FileSystem.h"

#include <ShlObj.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Platform/Base/IFileSystem.h"
#include "Oniun/Platform/Windows/Win32Headers.h"
#include "Oniun/Platform/Windows/Win32Platform.h"
#include "Oniun/Renderer/RendererLayer.h"

namespace Oniun
{
    String FileSystem::GetCurrentDirectory()
    {
        String buffer(256);
        GetCurrentDirectoryA((DWORD)buffer.Capacity(), buffer.Data());
        buffer.ReCalcLength();
        buffer.CorrectPathSlashes();
        return buffer;
    }

    bool FileSystem::CreateDirectory(const StringView& path)
    {
        DWORD fileAttributes = GetFileAttributesA(*path);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        {
            const DWORD error = GetLastError();
            if (error == ERROR_ACCESS_DENIED)
                return false;

            // Recursively create all sub directories
            uint64 slash = path.FindLast('/');
            if (slash != NO_POS)
            {
                String subPath(Slice(path.begin(), path.begin() + slash));
                if (CreateDirectory(subPath))
                    return true;
            }

            if (!CreateDirectoryA(path.Data(), nullptr))
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
        DWORD fileAttributes = GetFileAttributesA(*path);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }

        WIN32_FIND_DATAA  findData;
        HANDLE handle = FindFirstFileA(*(String(path) / '*'), &findData);
        do
        {
            StringView name(findData.cFileName);
            if (name == "." || name == "..")
                continue;

            String filePath = String(path) / name;
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
                if (!DeleteFileA(*filePath))
                {
                    LOG(Error, *Platform::GetLastErrorMessage());
                    FindClose(handle);
                    return false;
                }
            }
        }
        while (FindNextFileA(handle, &findData));
        FindClose(handle);

        if (!RemoveDirectoryA(*path))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }

        return true;
    }

    bool FileSystem::DirectoryExists(const StringView& path)
    {
        DWORD attrib = GetFileAttributesA(*path);
        return attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool FileSystem::GetDirectoryFiles(Array<String>& filePaths, const StringView& path,
                                       const StringView& searchPattern, DirectorySearch searchOpt)
    {
        if (searchOpt == DirectorySearch::CurrentOnly)
            return GetDirectoryFilesOnly(filePaths, path, searchPattern);
        return GetDirectoryFilesAll(filePaths, path, searchPattern);
    }

    bool FileSystem::GetChildDirectories(Array<String>& directories, const StringView& path)
    {
        WIN32_FIND_DATAA findData;
        HANDLE handle = FindFirstFileA(*(String(path) / '*'), &findData);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            FindClose(handle);
            return false;
        }

        do
        {
            StringView name(findData.cFileName);
            if (name == "." || name == "..")
                continue;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                directories.Add(String(path) / name);
        }
        while (FindNextFileA(handle, &findData));

        FindClose(handle);
        return true;
    }

    String FileSystem::GetSpecialDirectoryPath(SpecialDirectory type)
    {
        String result;
        KNOWNFOLDERID folderId;
        switch (type)
        {
        case SpecialDirectory::Desktop: folderId = FOLDERID_Desktop;
            break;
        case SpecialDirectory::Documents: folderId = FOLDERID_Documents;
            break;
        case SpecialDirectory::Downloads: folderId = FOLDERID_Downloads;
            break;
        case SpecialDirectory::Music: folderId = FOLDERID_Music;
            break;
        case SpecialDirectory::Pictures: folderId = FOLDERID_Pictures;
            break;
        case SpecialDirectory::Videos: folderId = FOLDERID_Videos;
            break;
        case SpecialDirectory::AppData: folderId = FOLDERID_RoamingAppData;
            break;
        case SpecialDirectory::LocalAppData: folderId = FOLDERID_LocalAppData;
            break;
        case SpecialDirectory::ProgramData: folderId = FOLDERID_ProgramData;
            break;
        case SpecialDirectory::Temporary:
            {
                char tempPath[MAX_PATH];
                if (GetTempPathA(MAX_PATH, tempPath))
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
            result.Resize(WideCharToMultiByte(CP_UTF8, 0, tempPath, -1, nullptr, 0, nullptr, nullptr));
            WideCharToMultiByte(CP_UTF8, 0, tempPath, -1, result.Data(), (DWORD)result.Length(), nullptr, nullptr);
            result.CorrectPathSlashes();
        }
        CoTaskMemFree(tempPath);
        return result;
    }

    bool FileSystem::DeleteFile(const StringView& path)
    {
        if (!DeleteFileA(*path))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }
        return true;
    }

    bool FileSystem::MoveFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting)
    {
        DWORD flags = MOVEFILE_COPY_ALLOWED | (overwriteExisting ? MOVEFILE_REPLACE_EXISTING : 0);
        if (!MoveFileExA(*srcPath, *destPath, flags))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }
        return true;
    }

    bool FileSystem::CopyFile(const StringView& destPath, const StringView& srcPath, bool overwriteExisting)
    {
        if (!CopyFileA(*srcPath, *destPath, !overwriteExisting))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }
        return true;
    }

    uint64 FileSystem::GetFileSize(const StringView& path)
    {
        WIN32_FILE_ATTRIBUTE_DATA attributes;
        if (GetFileAttributesExA(*path, GetFileExInfoStandard, &attributes))
        {
            if (!(attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                LARGE_INTEGER val;
                val.HighPart = attributes.nFileSizeHigh;
                val.LowPart = attributes.nFileSizeLow;
                return val.QuadPart;
            }
        }
        LOG(Error, *Platform::GetLastErrorMessage());
        return 0;
    }

    bool FileSystem::FileExists(const StringView& path)
    {
        DWORD attribute = GetFileAttributesA(*path);
        return attribute != INVALID_FILE_ATTRIBUTES && !(attribute & FILE_ATTRIBUTE_DIRECTORY);
    }

    String FileSystem::OpenDialog(const StringView& openPath, const StringView& filter, const StringView& title)
    {
        String result(MAX_PATH);

        OPENFILENAMEA openFileName;
        ZeroMemory(&openFileName, sizeof(openFileName));
        openFileName.lStructSize = sizeof(openFileName);
        openFileName.lpstrFilter = filter.Data();
        openFileName.lpstrFile = result.Data();
        openFileName.nMaxFile = MAX_PATH;
        openFileName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ENABLESIZING;
        openFileName.lpstrTitle = title.Data();
        openFileName.lpstrInitialDir = openPath.Data();

        if (GetOpenFileNameA(&openFileName) != 0)
        {
            result.ReCalcLength();
            result.CorrectPathSlashes();
        }
        return result;
    }

    Array<String> FileSystem::OpenDialogMultiple(const StringView& openPath, const StringView& filter, const StringView& title)
    {
        Array<String> results;
        String buffer(200 * MAX_PATH);

        OPENFILENAMEA openFileName;
        ZeroMemory(&openFileName, sizeof(openFileName));
        openFileName.lStructSize = sizeof(openFileName);
        openFileName.lpstrFilter = filter.Data();
        openFileName.lpstrFile = buffer.Data();
        openFileName.nMaxFile = static_cast<DWORD>(buffer.Capacity());
        openFileName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ENABLESIZING | OFN_ALLOWMULTISELECT;
        openFileName.lpstrTitle = title.Data();
        openFileName.lpstrInitialDir = openPath.Data();

        if (GetOpenFileNameA(&openFileName) != 0)
        {
            buffer.ReCalcLength();
            const char* ptr = buffer.Data();
            String directory(ptr);

            ptr += openFileName.nFileOffset;
            while (*ptr)
            {
                String path(directory / ptr);
                path.CorrectPathSlashes();
                results.Add(Memory::Move(path));
                ptr += StringUtils::Length(ptr) + 1;
            }
        }
        return results;
    }

    String FileSystem::SaveDialog(const StringView& openPath, const StringView& filter, const StringView& title)
    {
        String result(MAX_PATH);

        OPENFILENAMEA openFileName;
        ZeroMemory(&openFileName, sizeof(openFileName));
        openFileName.lStructSize = sizeof(openFileName);
        openFileName.lpstrFilter = filter.Data();
        openFileName.lpstrFile = result.Data();
        openFileName.nMaxFile = MAX_PATH;
        openFileName.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ENABLESIZING;
        openFileName.lpstrTitle = title.Data();
        openFileName.lpstrInitialDir = openPath.Data();

        if (GetSaveFileNameA(&openFileName) != 0)
        {
            result.ReCalcLength();
            result.CorrectPathSlashes();
        }
        return result;
    }

    String FileSystem::PickDirectoryDialog(const StringView& openPath, const StringView& title)
    {
        String result(MAX_PATH);

        BROWSEINFOA browseInfo;
        ZeroMemory(&browseInfo, sizeof(browseInfo));
        browseInfo.hwndOwner = nullptr;
        browseInfo.pidlRoot = nullptr;
        browseInfo.pszDisplayName = nullptr;
        browseInfo.lpszTitle = title.Data();
        browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        browseInfo.lpfn = nullptr;
        browseInfo.lParam = NULL;
        browseInfo.iImage = 0;

        LPITEMIDLIST itemIdList = SHBrowseForFolderA(&browseInfo);
        if (itemIdList != nullptr)
        {
            char path[MAX_PATH];
            if (SHGetPathFromIDListA(itemIdList, path))
            {
                result = path;
                result.CorrectPathSlashes();
            }
            IMalloc* pMalloc;
            if (SUCCEEDED(SHGetMalloc(&pMalloc)))
            {
                pMalloc->Free(itemIdList);
                pMalloc->Release();
            }
        }
        return result;
    }

    void FileSystem::OpenFileExplorer(const StringView& openPath)
    {
        RendererLayer* renderer = Engine::GetLayer<RendererLayer>();
        ASSERT(renderer);
        HWND handle = glfwGetWin32Window(renderer->GetWindow()->GetInternalWindow());
        ShellExecuteA(handle, "explore", *openPath, nullptr, nullptr, SW_SHOWNORMAL);
    }

    bool FileSystem::GetDirectoryFilesOnly(Array<String>& filePaths, const StringView& path, const StringView& searchPattern)
    {
        // Combine the path and the pattern into one string and initialize windows search handle
        WIN32_FIND_DATAA findData;
        HANDLE handle = FindFirstFileA(*(String(path) / searchPattern), &findData);
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
            if (name == "." || name == "..")
                continue;

            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                filePaths.Add(String(path) / name);
        }
        while (FindNextFileA(handle, &findData));

        FindClose(handle);
        return true;
    }

    bool FileSystem::GetDirectoryFilesAll(Array<String>& filePaths, const StringView& path, const StringView& searchPattern)
    {
        GetDirectoryFilesOnly(filePaths, path, searchPattern);

        // Combine the path and the pattern into one string and initialize windows search handle
        WIN32_FIND_DATAA findData;
        HANDLE handle = FindFirstFileA(*(String(path) / '*'), &findData);
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
            if (name == "." || name == "..")
                continue;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!GetDirectoryFilesAll(filePaths, String(path) / name, searchPattern))
                {
                    FindClose(handle);
                    return false;
                }
            }
        }
        while (FindNextFileA(handle, &findData));

        FindClose(handle);
        return false;
    }
}

#endif
