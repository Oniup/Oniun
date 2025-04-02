#pragma once

namespace Oniun
{
    enum class TextureMode
    {
        Invalid = -1,
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MaxCount,
    };

    enum class TextureFilter
    {
        Invalid = -1,
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear,
        MaxCount,
    };

    struct ITexture
    {
        TextureMode Modes;
        TextureFilter FilterMin;
        TextureFilter FilterMag;
        int32 Width;
        int32 Height;
        int32 Channels;

        virtual ~ITexture()
        {
        }

        virtual void Destroy() = 0;
        virtual bool IsValid() = 0;
    };
}
