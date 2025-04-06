#include "Oniun/RHI/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include "Oniun/Core/Logger.h"
#include "Oniun/Platform/File.h"
#include "Oniun/Platform/FileSystem.h"

namespace Oniun::RHI
{
    Pair<ShaderStage, String> Shader::GetSourceFromFile(const StringView& path)
    {
        Pair result(ShaderStage::Invalid, String::Empty);
        File file(path, FileAccess::Read);
        if (!file.IsOpen())
            return result;

        String src;
        src.Resize(file.GetSize());
        if (!file.Read(src.Data(), file.GetSize()))
            return result;

        Slice fileExt(FileSystem::GetFileExtensionFromPath(path));
        if (fileExt.IsEmpty())
        {
            LOG(Error, "Failed to get shader source stage from \"{}\" as it doesn't have a file extension", path);
            return result;
        }

        FixedArray<StringView, 4> shaderExtensions = {
            ".vert",
            ".frag",
            ".geom",
            ".comp"
        };
        for (uint64 i = (uint64)ShaderStage::Vertex; i < (uint64)ShaderStage::MaxCount; ++i)
        {
            if (shaderExtensions[i] == StringView(fileExt))
            {
                result.Key = (ShaderStage)i;
                result.Value = Memory::Move(src);
                return result;
            }
        }
        LOG(Error, "Invalid shader extension, unable to determine shader stage from \"{}\"", path);
        return result;
    }

    Shader::Shader()
        : m_GpuId(NO_POS)
    {
    }

    Shader::Shader(Shader&& shader)
        : m_GpuId(shader.m_GpuId)
    {
        shader.m_GpuId = NO_POS;
    }

    Shader& Shader::operator=(Shader&& shader)
    {
        Destroy();
        m_GpuId = shader.m_GpuId;
        shader.m_GpuId = NO_POS;
        return *this;
    }

    void Shader::Destroy()
    {
        if (m_GpuId != NO_POS)
        {
            glDeleteProgram(m_GpuId);
            m_GpuId = NO_POS;
        }
    }

    bool Shader::IsValid() const
    {
        return m_GpuId != NO_POS;
    }

    bool Shader::LoadFromSource(const Array<Pair<ShaderStage, String>>& sources)
    {
        Array<uint64> ids(sources.Count());
        auto destroyShaders = [&ids]()
        {
            for (uint64 shader : ids)
                glDeleteShader(shader);
        };

        for (auto&[stage, src] : sources)
        {
            GLenum glType;
            switch (stage)
            {
            case ShaderStage::Vertex:
                glType = GL_VERTEX_SHADER;
                break;
            case ShaderStage::Fragment:
                glType = GL_FRAGMENT_SHADER;
                break;
            case ShaderStage::Geometry:
                glType = GL_GEOMETRY_SHADER;
                break;
            case ShaderStage::Compute:
                glType = GL_COMPUTE_SHADER;
                break;
            default:
                LOG(Warning, "Cannot create shader, stage is invalid");
                destroyShaders();
                return false;
            }

            uint64 shader = glCreateShader(glType);
            const char* shaderSource = src.Data();
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);

            int32 compiled;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled != GL_TRUE)
            {
                constexpr int32 maxLength = 1024;
                char buffer[maxLength];
                int32 logLength;
                glGetShaderInfoLog(shader, maxLength, &logLength, buffer);

                LOG(Error, "Failed to compile {} shader:\nOpenGL Error: {}\nSource:\n{}", stage, StringView(buffer, logLength), src);
                destroyShaders();
                return false;
            }

            ids.Add(shader);
        }

        int32 program = glCreateProgram();
        for (int32 shader : ids)
            glAttachShader(program, shader);
        glLinkProgram(program);

        int32 linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE)
        {
            constexpr uint64 maxLength = 1024;
            char buffer[maxLength];
            int32 logLength;
            glGetProgramInfoLog(program, maxLength, &logLength, buffer);

            String sourceFiles;
            for (auto&[stage, src] : sources)
                Fmt::FormatTo(sourceFiles, "{}:\n{}\n", stage, src);
            LOG(Error, "Failed to link shader program:\nOpenGL Error: {}\nSources:\n{}", StringView(buffer, logLength), sourceFiles);
            program = NO_POS;
        }

        destroyShaders();
        m_GpuId = program;
        return (bool)linked;
    }

    bool Shader::LoadFromBinary(const Array<Pair<ShaderStage, Slice<char>>>& sources)
    {
        LOG(Fatal, "Implementation doesn't exist yet");
        return false;
    }

    void Shader::SetUniform(const StringView& location, int32 val)
    {
        glUniform1i(glGetUniformLocation(m_GpuId, *location), val);
    }

    void Shader::SetUniform(const StringView& location, float val)
    {
        glUniform1f(glGetUniformLocation(m_GpuId, *location), val);
    }

    void Shader::SetUniform(const StringView& location, glm::ivec2 vec)
    {
        glUniform2iv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::ivec3 vec)
    {
        glUniform3iv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::ivec4 vec)
    {
        glUniform4iv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::vec2 vec)
    {
        glUniform2fv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::vec3 vec)
    {
        glUniform3fv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::vec4 vec)
    {
        glUniform4fv(glGetUniformLocation(m_GpuId, *location), 1, &vec[0]);
    }

    void Shader::SetUniform(const StringView& location, glm::mat2 mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(m_GpuId, *location), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetUniform(const StringView& location, glm::mat3 mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_GpuId, *location), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetUniform(const StringView& location, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_GpuId, *location), 1, GL_FALSE, &mat[0][0]);
    }
}
