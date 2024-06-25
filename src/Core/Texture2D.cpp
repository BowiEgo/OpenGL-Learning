#include "Texture2D.h"

#include "stb_image/stb_image.h"

Texture2D::Texture2D(const std::string &type, const unsigned int rendererID)
    : m_Type(type), m_RendererID(rendererID)
{
    m_RendererID = rendererID;
}

Texture2D::Texture2D(const std::string &path, const TextureOptions &options)
    : m_Type("texture_default"), m_FilePath(path), m_LocalBuffer(nullptr), m_Options(options),
      m_Width(0), m_Height(0), m_Channels(0)
{
    SetupTexture2D(path, options);
}

Texture2D::Texture2D(const std::string &type, const std::string &path, const TextureOptions &options)
    : m_Type(type), m_FilePath(path), m_LocalBuffer(nullptr), m_Options(options),
      m_Width(0), m_Height(0), m_Channels(0)
{
    SetupTexture2D(path, options);
}

Texture2D::~Texture2D()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::SetupTexture2D(const std::string &path, const TextureOptions &options)
{
    stbi_set_flip_vertically_on_load(options.flip);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Options.minFilter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Options.magFilter));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Options.wrapS));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Options.wrapT));

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
    else if (m_Channels == 1)
    {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }

    CORE_INFO("{0}, {1}, {2}, {3}, {4}, {5}",path, m_Channels, internalFormat, m_Width, m_Height, dataFormat);
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

void Texture2D::SetupVoidTexture2D()
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture2D* Texture2D::CreateVoidTexture(const std::string &type)
{
    return new Texture2D(type, "../res/textures/error.jpg");
}

void Texture2D::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::SetWrapping(GLenum pname, GLint param) const
{
    Bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, pname, param));
}
