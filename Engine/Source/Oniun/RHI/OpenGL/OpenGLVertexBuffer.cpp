#include "Oniun/RHI/OpenGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

#include "Oniun/Core/Logger.h"

namespace Oniun::RHI
{
    VertexBuffer::VertexBuffer()
        : m_VertexArray(NO_POS), m_AttributeCount(0), m_NextAttributeOffset(0)
    {
        m_Buffers[GpuBuffer_Vertex] = NO_POS;
        m_Buffers[GpuBuffer_Element] = NO_POS;
    }

    bool VertexBuffer::Create(uint64 stride, BufferDrawType drawType, bool useElementBuffer)
    {
        if (stride == 0 || stride == NO_POS)
            return false;

        m_Stride = stride;
        m_DrawType = drawType;

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        glGenBuffers(useElementBuffer ? 2 : 1, m_Buffers);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[GpuBuffer_Vertex]);
        if (useElementBuffer)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[GpuBuffer_Element]);

        glBindVertexArray(0);
        return true;
    }

    void VertexBuffer::Destroy()
    {
        if (m_VertexArray != NO_POS)
        {
            glDeleteVertexArrays(1, &m_VertexArray);
            m_VertexArray = NO_POS;
            glDeleteBuffers(m_Buffers[GpuBuffer_Element] != NO_POS ? 2 : 1, m_Buffers);
            for (uint32& buffer : m_Buffers)
                buffer = NO_POS;
            m_AttributeCount = 0;
        }
    }

    bool VertexBuffer::ElementBufferEnabled()
    {
        return m_Buffers[GpuBuffer_Element] != NO_POS;
    }

    bool VertexBuffer::IsValid()
    {
        return m_VertexArray != NO_POS;
    }

    void VertexBuffer::AddAttribute(uint32 count, bool normalize)
    {
        if ((sizeof(float)) * m_NextAttributeOffset + count > m_Stride)
        {
            LOG(Error, "Cannot add attribute as it increases past the stride length {}", m_Stride);
            return;
        }

        glBindVertexArray(m_VertexArray);
        glEnableVertexAttribArray(m_AttributeCount);
        glVertexAttribPointer(m_AttributeCount, count, GL_FLOAT, normalize, m_Stride, (void*)(sizeof(GL_FLOAT) * m_NextAttributeOffset));
        glBindVertexArray(0);
        m_NextAttributeOffset += count;
        ++m_AttributeCount;
    }

    void VertexBuffer::SetData(GpuBufferType type, const byte* bufferData, uint64 byteSize)
    {
        glBindVertexArray(m_VertexArray);
        int32 glType = type == GpuBuffer_Vertex ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER ;
        int32 glDrawType = m_DrawType == BufferDrawType::DynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        glBufferData(glType, byteSize, bufferData, glDrawType);
        glBindVertexArray(0);
    }
}
