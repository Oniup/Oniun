#pragma once

#include "Oniun/Renderer/GpuResource.h"

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
        None,
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear,
        MaxCount,
    };

    namespace RHI
    {
        class ITexture
        {
        public:
            virtual ~ITexture()
            {
            }

        public:
            virtual void Destroy() = 0;
            virtual bool IsValid() = 0;

            virtual bool CreateFromImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) = 0;
            virtual bool CreateEmpty(ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) = 0;
            virtual bool LoadImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) = 0;
        };
    }
}
