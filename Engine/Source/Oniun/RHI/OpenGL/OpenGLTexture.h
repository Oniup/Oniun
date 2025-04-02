#pragma once

#include "Oniun/RHI/Base/ITexture.h"

namespace Oniun
{
    struct Texture : ITexture
    {
        uint32 GpuId;

        Texture();

        // TODO: Move the reading image from file to asset manager
        static Texture CreateFromImage(const StringView& imageSource, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag);

        void Destroy() override;
        bool IsValid() override;
    };
}
