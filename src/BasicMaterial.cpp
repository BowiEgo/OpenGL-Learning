#include "BasicMaterial.h"

#include "FileSystem/FileSystem.h"

BasicMaterial::BasicMaterial()
{
    std::string vertexSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
    std::string fragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragSrc);
    m_Shader->Bind();
    m_Shader->SetUniformVec3("u_Color", { 1.0f, 1.0f, 1.0f });
}

BasicMaterial::~BasicMaterial()
{
}

void BasicMaterial::SetColor(const float vector[3])
{
    m_Shader->Bind();
    m_Shader->SetUniformVec3("u_Color", vector);
}
