#pragma once

#include "Renderer.h"

struct TextureOptions {
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    GLint minFilter = GL_NEAREST;
    GLint magFilter = GL_LINEAR;
};

class Texture
{
public:
    Texture(const std::string& path, const TextureOptions& options = TextureOptions());
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetWrapping(GLenum pname, GLint param) const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels; // BPP = Bits Per Pixel
    TextureOptions m_Options;
};
