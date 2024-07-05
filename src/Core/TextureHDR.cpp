#include "TextureHDR.h"

#include "stb_image/stb_image.h"

TextureHDR::TextureHDR(const std::string &type, const unsigned int rendererID)
    : m_Type(type), m_RendererID(rendererID)
{
    m_RendererID = rendererID;
}

TextureHDR::TextureHDR(const std::string &path)
    : m_Type("texture_default"), m_FilePath(path), m_LocalBuffer(nullptr),
      m_Width(0), m_Height(0), m_Channels(0)
{
    SetupTextureHDR(path);
}

TextureHDR::TextureHDR(const std::string &type, const std::string &path)
    : m_Type(type), m_FilePath(path), m_LocalBuffer(nullptr),
      m_Width(0), m_Height(0), m_Channels(0)
{
    SetupTextureHDR(path);
}

TextureHDR::~TextureHDR()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void TextureHDR::SetupTextureHDR(const std::string &path)
{
    stbi_set_flip_vertically_on_load(true);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    CORE_INFO("{0}, {1}, {2}, {3}, {4}, {5}", path, m_Channels, m_Width, m_Height);
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}


void TextureHDR::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void TextureHDR::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
