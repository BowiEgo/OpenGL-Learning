#include "TextureCubemap.h"

#include "stb_image/stb_image.h"

TextureCubemap::TextureCubemap(const std::string &type)
    : m_Type(type)
{
    SetupEmptyTextureCubemap();
}

TextureCubemap::TextureCubemap(const std::string &type, const unsigned int rendererID)
    : m_Type(type), m_RendererID(rendererID)
{
}

TextureCubemap::TextureCubemap(const std::string& type, const std::vector<std::string> facePaths, const TextureCubemapOptions &options)
    : m_Type(type), m_FilePaths(facePaths), m_LocalBuffer(nullptr), m_Options(options),
      m_Width(0), m_Height(0), m_Channels(0)
{
    SetupTextureCubemap(facePaths, options);
}

TextureCubemap::~TextureCubemap()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void TextureCubemap::SetupTextureCubemap(const std::vector<std::string> facePaths, const TextureCubemapOptions &options)
{
    stbi_set_flip_vertically_on_load(options.flip);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_Options.minFilter));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_Options.magFilter));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_Options.wrapS));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_Options.wrapT));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_Options.wrapR));

    for (unsigned int i = 0; i < facePaths.size(); i++)
    {
        m_LocalBuffer = stbi_load(facePaths[i].c_str(), &m_Width, &m_Height, &m_Channels, 0);

        GLenum internalFormat = 0, dataFormat = 0;
        if (m_Channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        CORE_INFO("TextureCubemap: {0}, {1}, {2}, {3}, {4}, {5}", facePaths[i], m_Channels, internalFormat, m_Width, m_Height, dataFormat);
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));

        if (m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
        else
            CORE_ERROR("Cubemap texture failed to load at path: {0}", facePaths[i]);
    }

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void TextureCubemap::SetupEmptyTextureCubemap()
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    for (unsigned int i = 0; i < 6; i++)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    }

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

Ref<TextureCubemap> TextureCubemap::CreateEmptyTexture(const std::string &type, const TextureCubemapOptions &options)
{
    return std::make_shared<TextureCubemap>(type);
}

void TextureCubemap::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void TextureCubemap::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void TextureCubemap::SetWrapping(GLenum pname, GLint param) const
{
    Bind();
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, pname, param));
}
