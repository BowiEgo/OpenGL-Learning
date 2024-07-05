#pragma once

#include "pch.h"
#include "Core/Renderer.h"

#include "Texture.h"

class TextureHDR : public Texture
{
public:
    TextureHDR(const std::string& type, const unsigned int rendererID);
    TextureHDR(const std::string& path);
    TextureHDR(const std::string& type, const std::string& path);
    ~TextureHDR();

    void SetupTextureHDR(const std::string& path);

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind() const override;

    virtual inline int GetWidth() const override { return m_Width; }
    virtual inline int GetHeight() const override { return m_Height; }
    virtual inline unsigned int GetID() const override { return m_RendererID; }
    virtual inline std::string GetType() const override { return m_Type; }
    inline std::string GetFilePath() const { return m_FilePath; }
private:
    unsigned int m_RendererID;
    std::string m_Type = "";
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
