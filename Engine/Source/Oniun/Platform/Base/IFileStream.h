#pragma once

#include "Oniun/Core/BaseTypes.h"

namespace Onu
{
    enum FileMode
    {
        FileStreamMode_Create,
        FileStreamMode_CreateNewOnly,
        FileStreamMode_Open,
        FileStreamMode_OpenExistingOnly,
    };

    enum FileAccess
    {
        FileAccess_Read = 1 << 0,
        FileAccess_Write = 1 << 1,
        FileAccess_Append = 1 << 2,
    };

    class IFileStream
    {
    public:
        virtual ~IFileStream()
        {
        }

        virtual bool Read(void* buffer, uint64 bytesToRead, uint64* bytesRead = nullptr) = 0;
        virtual bool Write(void* buffer, uint64 bytesToWrite, uint64* bytesWritten = nullptr) = 0;
        virtual bool Close() = 0;

        virtual uint64 GetSize() const = 0;
    };
}
