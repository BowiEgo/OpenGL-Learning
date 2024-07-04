#pragma once

#include "pch.h"
#include "Core/Renderer.h"

#include "Texture.h"

struct TextureOptions {
    GLint minFilter = GL_NEAREST;
    GLint magFilter = GL_LINEAR;
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    bool flip = true;
    bool HDR = false;
};

class Texture2D : public Texture
{
public:
    Texture2D(const std::string& type, const unsigned int rendererID);
    Texture2D(const std::string& path, const TextureOptions& options = TextureOptions());
    Texture2D(const std::string& type, const std::string& path, const TextureOptions& options = TextureOptions());
    ~Texture2D();

    void SetupTexture2D(const std::string& path, const TextureOptions& options = TextureOptions());
    void SetupVoidTexture2D();

    // static unsigned int Create(const std::string& type, const std::string& path, const TextureOptions& options = TextureOptions());
    static Texture2D* CreateVoidTexture(const std::string& type);

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind() const override;

    virtual void SetWrapping(GLenum pname, GLint param) const override;

    virtual inline int GetWidth() const override { return m_Width; }
    virtual inline int GetHeight() const override { return m_Height; }
    virtual inline unsigned int GetID() const override { return m_RendererID; }
    virtual inline std::string GetType() const override { return m_Type; }
    inline std::string GetFilePath() const { return m_FilePath; }
private:
    TextureOptions m_Options;
    unsigned int m_RendererID;
    std::string m_Type = "";
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
