#include "Oniun/RHI/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

#include "stb/stb_image.h"

namespace Oniun
{
    Texture::Texture()
        : GpuId(NO_POS)
    {
    }

    Texture Texture::CreateFromImage(const StringView& imageSource, TextureMode mode, TextureFilter filterMin,
                                     TextureFilter filterMag)
    {
        Texture texture;
        uint8* imageData = stbi_load(*imageSource, &texture.Width, &texture.Height, &texture.Channels, 0);
        if (!imageData)
        {
        }
        // TODO: Finish ...
    }

    void Texture::Destroy()
    {
        if (GpuId != NO_POS)
        {
            glDeleteTextures(1, &GpuId);
            GpuId = NO_POS;
        }
    }

    bool Texture::IsValid()
    {
        return GpuId != NO_POS;
    }
}
