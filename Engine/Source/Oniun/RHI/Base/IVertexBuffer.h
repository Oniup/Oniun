#pragma once

#include "Oniun/Renderer/GpuResource.h"

namespace Oniun::RHI
{
    class IVertexBuffer
    {
    public:
        IVertexBuffer()
            : m_DrawType(), m_Stride(0), m_TotalElementsPerVertexCount(0)
        {
        }

        virtual ~IVertexBuffer()
        {
        }

        virtual bool Create(uint64 stride, BufferDrawType drawType, bool useElementBuffer) = 0;
        virtual void Destroy() = 0;

        virtual bool IsValid() = 0;
        virtual bool ElementBufferEnabled() = 0;

    public:
        FORCE_INLINE BufferDrawType GetDrawType() const
        {
            return m_DrawType;
        }

        FORCE_INLINE uint64 GetStride() const
        {
            return m_Stride;
        }

        FORCE_INLINE uint64 GetTotalElementsPerVertexCount() const
        {
            return m_TotalElementsPerVertexCount;
        }

    protected:
        BufferDrawType m_DrawType;
        uint64 m_Stride;
        uint64 m_TotalElementsPerVertexCount;
    };
}
