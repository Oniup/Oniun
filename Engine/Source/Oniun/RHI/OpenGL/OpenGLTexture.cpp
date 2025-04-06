#include "Oniun/RHI/OpenGL/OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Oniun/Core/Logger.h"

namespace Oniun::RHI
{
    uint8* Texture::ReadImageData(const StringView& path, int32* width, int32* height, ColorFormat* format)
    {
        int32 channels;
        uint8* imageData = stbi_load(*path, width, height, &channels, 0);
        if (!imageData)
            LOG(Error, "Failed to load image from \"{}\"", path);

        switch (channels)
        {
        case 1:
            *format = ColorFormat::Red;
            break;
        case 2:
            *format = ColorFormat::RG;
            break;
        case 3:
            *format = ColorFormat::RGB;
            break;
        case 4:
            *format = ColorFormat::RGBA;
            break;
        default:
            stbi_image_free(imageData);
            imageData = nullptr;
            LOG(Error, "Invalid color format ({} channels) from file \"{}\"", channels, path);
            break;
        }
        return imageData;
    }

    void Texture::DestroyImageData(uint8* imageData)
    {
        if (imageData)
            stbi_image_free(imageData);
    }

    Texture::Texture()
        : m_GpuId(NO_POS)
    {
    }

    Texture::Texture(Texture&& texture)
        : m_GpuId(texture.m_GpuId)
    {
        texture.m_GpuId = NO_POS;
    }

    Texture& Texture::operator=(Texture&& texture)
    {
        Destroy();
        m_GpuId = texture.m_GpuId;
        texture.m_GpuId = NO_POS;
        return *this;
    }

    void Texture::Destroy()
    {
        if (m_GpuId != NO_POS)
        {
            glDeleteTextures(1, &m_GpuId);
            m_GpuId = NO_POS;
        }
    }

    bool Texture::IsValid()
    {
        return m_GpuId != NO_POS;
    }

    bool Texture::CreateFromImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag)
    {
        if (!imageData)
        {
            LOG(Error, "Image data is null, cannot create texture");
            return false;
        }
        glGenTextures(1, &m_GpuId);
        if (!LoadImage(imageData, format, width, height, mode, filterMin, filterMag))
        {
            glDeleteTextures(1, &m_GpuId);
            return false;
        }
        return true;
    }

    bool Texture::CreateEmpty(ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag)
    {
        uint32 glFormat = GetPlatformFormat(format);
        if (glFormat == NO_POS)
        {
            LOG(Error, "Invalid image format with {} channels, cannot create texture", format);
            return false;
        }
        glGenTextures(1, &m_GpuId);
        if (!LoadImage(nullptr, format, width, height, mode, filterMin, filterMag))
        {
            glDeleteTextures(1, &m_GpuId);
            return false;
        }
        return true;
    }

    bool Texture::LoadImage(const uint8* imageData, ColorFormat format, int32 width, int32 height, TextureMode mode, TextureFilter filterMin, TextureFilter filterMag)
    {
        ASSERT(m_GpuId != NO_POS);
        uint32 glFormat = GetPlatformFormat(format);
        if (glFormat == NO_POS)
        {
            LOG(Error, "Invalid image format with {} channels, cannot create texture", format);
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, m_GpuId);
        if (!SetTextureParameters(mode, filterMin, filterMag))
            return false;

        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, imageData);
        if (filterMag > TextureFilter::Linear)
            glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    bool Texture::SetTextureParameters(TextureMode mode, TextureFilter filterMin, TextureFilter filterMag)
    {
        ASSERT(m_GpuId != NO_POS);

        uint32 glMode = GetPlatformMode(mode);
        uint32 glFilterMin = GetPlatformFilter(filterMin);
        uint32 glFilterMag = GetPlatformFilter(filterMag);

        if (glMode == NO_POS)
        {
            LOG(Error, "Texture mode is invalid, cannot create texture");
            return false;
        }
        if (glFilterMin == NO_POS)
        {
            LOG(Error, "The min texture filter is set to none, cannot create texture. Only the mag filter can be none");
            return false;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glMode);
        if (glFilterMag != NO_POS)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterMin);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterMag);
        }

        return true;
    }

    uint32 Texture::GetPlatformFormat(ColorFormat format)
    {
        switch (format)
        {
        case ColorFormat::Red:
            return GL_RED;
        case ColorFormat::RG:
            return GL_RG;
        case ColorFormat::RGB:
            return GL_RGB;
        case ColorFormat::RGBA:
            return GL_RGBA;
        default:
            return NO_POS;
        }
    }

    uint32 Texture::GetPlatformMode(TextureMode mode)
    {
        switch (mode)
        {
        case TextureMode::Repeat:
            return GL_REPEAT;
        case TextureMode::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case TextureMode::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureMode::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            return NO_POS;
        }
    }

    uint32 Texture::GetPlatformFilter(TextureFilter filter)
    {
        switch (filter)
        {
        case TextureFilter::Nearest:
            return GL_NEAREST;
        case TextureFilter::Linear:
            return GL_LINEAR;
        case TextureFilter::NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return NO_POS;
        }
    }
}
