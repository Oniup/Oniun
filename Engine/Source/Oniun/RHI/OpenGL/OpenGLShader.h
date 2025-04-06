#pragma once

#include "Oniun/Renderer/GpuResource.h"
#include "Oniun/RHI/Base/IShader.h"

namespace Oniun::RHI
{
    class Shader : public IShader
    {
    public:
        // TODO: Move to asset manager later
        static Pair<ShaderStage, String> GetSourceFromFile(const StringView& path);

    public:
        Shader();
        Shader(Shader&& shader);
        Shader& operator=(Shader&& shader);

    public:
        void Destroy() override;
        bool IsValid() const override;

        bool LoadFromSource(const Array<Pair<ShaderStage, String>>& sources) override;
        bool LoadFromBinary(const Array<Pair<ShaderStage, Slice<char>>>& sources) override;

        FORCE_INLINE uint32 GetGpuId() const
        {
            return m_GpuId;
        }

        void SetUniform(const StringView& location, int32 val);
        void SetUniform(const StringView& location, float val);

        void SetUniform(const StringView& location, glm::ivec2 vec);
        void SetUniform(const StringView& location, glm::ivec3 vec);
        void SetUniform(const StringView& location, glm::ivec4 vec);

        void SetUniform(const StringView& location, glm::vec2 vec);
        void SetUniform(const StringView& location, glm::vec3 vec);
        void SetUniform(const StringView& location, glm::vec4 vec);

        void SetUniform(const StringView& location, glm::mat2 mat);
        void SetUniform(const StringView& location, glm::mat3 mat);
        void SetUniform(const StringView& location, glm::mat4 mat);

    private:
        uint32 m_GpuId;
    };
}
