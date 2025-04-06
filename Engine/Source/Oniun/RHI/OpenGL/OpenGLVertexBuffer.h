#pragma once

#include "Oniun/RHI/Base/IVertexBuffer.h"

namespace Oniun::RHI
{
    class VertexBuffer : public IVertexBuffer
    {
    public:
        VertexBuffer();

    public:
        FORCE_INLINE uint32 GetGpuId() const
        {
            return m_VertexArray;
        }

    public:
        bool Create(uint64 stride, BufferDrawType drawType, bool useElementBuffer) override;
        void Destroy() override;

        bool ElementBufferEnabled() override;
        bool IsValid() override;

        void AddAttribute(uint32 count, bool normalize = false);
        void SetData(GpuBufferType type, const byte* bufferData, uint64 byteSize);

    private:
        uint32 m_VertexArray;
        uint32 m_Buffers[GpuBuffer_MaxCount];
        uint64 m_AttributeCount;
        uint64 m_NextAttributeOffset;
    };
}
