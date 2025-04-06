#pragma once

namespace Oniun::RHI
{
    class IShader
    {
    public:
        virtual ~IShader()
        {
        }

        virtual void Destroy() = 0;
        virtual bool IsValid() const = 0;

        virtual bool LoadFromSource(const Array<Pair<ShaderStage, String>>& sources) = 0;
        virtual bool LoadFromBinary(const Array<Pair<ShaderStage, Slice<char>>>& sources) = 0;
    };
}
