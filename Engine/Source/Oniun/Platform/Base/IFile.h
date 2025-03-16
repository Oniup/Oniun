#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/ClassConstraints.h"

enum class FileMode : uint32
{
    /// Creates a new file; fails if the file already exists.
    CreateNew = 1,
    /// Creates a new file; overrides the file if it already exists.
    CreateAlways = 2,
    /// Opens an existing file; fails if no file exists.
    OpenExisting = 3,
    /// Opens a file if it exists; creates a new one if it no file exists.
    OpenAlways = 4,
    /// Opens an existing file and truncates it to zero bytes; fails if no file exists. Used to delete the contents
    /// of a file.
    TruncateExisting = 5,
};

enum class FileAccess : uint32
{
    Read = 0x80000000,
    Write = 0x40000000,
    ReadWrite = Read | Write,
};

class IFile : public INonCopyable
{

public:
    IFile(bool owns)
        : m_Owns(owns)
    {
    }

    virtual ~IFile()
    {
    }

public:
    virtual void Open(const StringView& path, FileAccess access, FileMode mode = FileMode::OpenAlways) = 0;

    virtual bool Read(void* buffer, uint32 bytesToRead, uint32* bytesRead = nullptr) = 0;
    virtual bool Write(const void* buffer, uint32 bytesToWrite, uint32* bytesWritten = nullptr) = 0;

    FORCE_INLINE bool OwnsPointer() const
    {
        return m_Owns;
    }

    virtual void Close() = 0;
    virtual bool Flush() = 0;
    virtual bool IsOpen() const = 0;

    virtual uint32 GetSize() const = 0;
    virtual uint32 GetPosition() const = 0;
    virtual void SetPosition(uint32 seek) const = 0;
    virtual DateTime LastWriteTime() const = 0;

protected:
    bool m_Owns;
};
