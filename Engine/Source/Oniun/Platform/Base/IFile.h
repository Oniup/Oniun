#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/ClassRestrictions.h"

namespace Onu
{
    enum FileMode
    {
        /// Creates a new file; fails if the file already exists.
        FileMode_CreateNew         = 1,
        /// Creates a new file; overrides the file if it already exists.
        FileMode_CreateAlways      = 2,
        /// Opens an existing file; fails if no file exists.
        FileMode_OpenExisting      = 3,
        /// Opens a file if it exists; creates a new one if it no file exists.
        FileMode_OpenAlways        = 4,
        /// Opens an existing file and truncates it to zero bytes; fails if no file exists. Used to delete the contents
        /// of a file.
        FileMode_TruncateExisting  = 5,
    };

    enum FileAccess
    {
        FileAccess_Read = 0x80000000,
        FileAccess_Write = 0x40000000,
        FileAccess_ReadWrite = FileAccess_Read | FileAccess_Write,
    };

    class IFile : public INonCopyable
    {
    public:
        virtual ~IFile()
        {
        }

        virtual bool Read(void* buffer, uint32 bytesToRead, uint32* bytesRead = nullptr) = 0;
        virtual bool Write(const void* buffer, uint32 bytesToWrite, uint32* bytesWritten = nullptr) = 0;

        virtual void Close() = 0;
        virtual bool Flush() = 0;
        virtual bool IsOpen() const = 0;

        virtual uint32 GetSize() const = 0;
        virtual uint32 GetPosition() const = 0;
        virtual void SetPosition(uint32 seek) const = 0;
        virtual DateTime LastWriteTime() const = 0;
    };
}
