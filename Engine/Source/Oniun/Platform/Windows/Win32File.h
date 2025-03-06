#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Base/IFile.h"

class File : public IFile
{
public:
    File();
    File(File&& file);
    File(void* handle, bool owns = true);
    File(const StringView& path, FileAccess access, FileMode mode = FileMode::OpenAlways);
    ~File() override;

public:
    File& operator=(File&& file);

public:
    bool Read(void* buffer, uint32 bytesToRead, uint32* bytesRead = nullptr) override;
    bool Write(const void* buffer, uint32 bytesToWrite, uint32* bytesWritten = nullptr) override;

    void Close() override;
    bool Flush() override;
    bool IsOpen() const override;

    uint32 GetSize() const override;
    uint32 GetPosition() const override;
    void SetPosition(uint32 seek) const override;
    DateTime LastWriteTime() const override;

private:
    void* m_Handle;
};

#endif
