#pragma once

#include "Oniun/RHI/Base/ITexture.h"

namespace Oniun::RHI
{
    class Texture : public ITexture
    {
    public:
        // TODO: Move the reading image from file to asset manager
        static uint8* ReadImageData(const StringView& path, int32* width, int32* height, ColorFormat* format);
        static void DestroyImageData(uint8* imageData);

    public:
        Texture();
        Texture(Texture&& texture);
        Texture& operator=(Texture&& texture);

        FORCE_INLINE uint32 GetGpuId() const
        {
            return m_GpuId;
        }

        void Destroy() override;
        bool IsValid() override;

        bool CreateFromImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) override;
        bool CreateEmpty(ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) override;
        bool LoadImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag) override;

    private:
        bool SetTextureParameters(TextureMode mode, TextureFilter filterMin, TextureFilter filterMag);
        uint32 GetPlatformFormat(ColorFormat format);
        uint32 GetPlatformMode(TextureMode mode);
        uint32 GetPlatformFilter(TextureFilter filter);

    private:
        uint32 m_GpuId;
    };
}
