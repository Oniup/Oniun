#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32File.h"

#include "Oniun/Core/DateTime.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Platform/Windows/Win32Headers.h"
#include "Oniun/Platform/Windows/Win32Platform.h"

namespace Onu
{
    File::File()
        : IFile(true), m_Handle(INVALID_HANDLE_VALUE)
    {
    }

    File::File(File&& file)
        : IFile(file.m_Owns), m_Handle(file.m_Handle)
    {
        file.m_Handle = INVALID_HANDLE_VALUE;
    }

    File::File(void* handle, bool owns)
        : IFile(owns), m_Handle(handle)
    {
    }

    File::File(const StringView& path, FileAccess access, FileMode mode)
        : IFile(true), m_Handle(INVALID_HANDLE_VALUE)
    {
        HANDLE handle = CreateFileW(*path, (DWORD)access, 0, nullptr, (DWORD)mode, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (handle == INVALID_HANDLE_VALUE)
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return;
        }
        m_Handle = handle;
    }

    File::~File()
    {
        File::Close();
    }

    File& File::operator=(File&& file)
    {
        Close();
        m_Handle = file.m_Handle;
        file.m_Handle = INVALID_HANDLE_VALUE;
        return *this;
    }

    bool File::Read(void* buffer, uint32 bytesToRead, uint32* bytesRead)
    {
        DWORD temp;
        if (ReadFile(m_Handle, buffer, bytesToRead, &temp, nullptr))
        {
            if (bytesRead)
                *bytesRead = temp;
            return true;
        }
        if (bytesRead)
            *bytesRead = 0;

        LOG(Error, *Platform::GetLastErrorMessage());
        return false;
    }

    bool File::Write(const void* buffer, uint32 bytesToWrite, uint32* bytesWritten)
    {
        DWORD temp;
        if (WriteFile(m_Handle, buffer, bytesToWrite, &temp, nullptr))
        {
            if (bytesWritten)
                *bytesWritten = temp;
            return true;
        }
        if (bytesWritten)
            *bytesWritten = 0;

        LOG(Error, *Platform::GetLastErrorMessage());
        return false;
    }

    void File::Close()
    {
        if (m_Owns && m_Handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_Handle);
            m_Handle = nullptr;
        }
    }

    bool File::Flush()
    {
        if (!FlushFileBuffers(m_Handle))
        {
            LOG(Error, *Platform::GetLastErrorMessage());
            return false;
        }
        return true;
    }

    bool File::IsOpen() const
    {
        return m_Handle != INVALID_HANDLE_VALUE;
    }

    uint32 File::GetSize() const
    {
        LARGE_INTEGER result;
        if (GetFileSizeEx(m_Handle, &result))
            return (uint32)result.QuadPart;

        LOG(Error, *Platform::GetLastErrorMessage());
        return 0;
    }

    uint32 File::GetPosition() const
    {
        return SetFilePointer(m_Handle, 0, nullptr, FILE_BEGIN);
    }

    void File::SetPosition(uint32 seek) const
    {
        SetFilePointer(m_Handle, seek, nullptr, FILE_BEGIN);
    }

    DateTime File::LastWriteTime() const
    {
        if (m_Handle)
        {
            FILETIME lastWriteTime;
            SYSTEMTIME sysTime;

            GetFileTime(m_Handle, nullptr, nullptr, &lastWriteTime);
            FileTimeToSystemTime(&lastWriteTime, &sysTime);

            return DateTime(sysTime.wSecond, sysTime.wMinute, sysTime.wHour, sysTime.wMonth, sysTime.wDay,
                            sysTime.wYear);
        }
        return DateTime();
    }
}

#endif
