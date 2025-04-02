#pragma once

#include "Oniun/RHI/Base/IShader.h"

namespace Oniun::RHI
{
    struct Shader : IShader
    {
        uint64 GpuId;

        Shader();

        // TODO: Move to asset manager later
        static Pair<ShaderType, String> GetSourceFromFile(const StringView& path);

        static Shader CreateFromSource(const Array<Pair<ShaderType, String>>& sources);
        static Shader CreateFromBinary(const Array<Pair<ShaderType, Slice<char>>>& sources);

        void Destroy() override;
        bool IsValid() const override;

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
    };
}
