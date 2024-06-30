#include "FramebufferTexture.h"

FramebufferTexture::FramebufferTexture(const FBOTextureOptions &options)
    : m_Options(options)
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLenum internalFormat = m_Options.HDR ? GL_RGB16 : GL_RGB, dataFormat = GL_RGB;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Options.Width, m_Options.Height, 0, dataFormat, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

FramebufferTexture::~FramebufferTexture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}
