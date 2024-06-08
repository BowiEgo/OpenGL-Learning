#include "BasicMaterial.h"

#include "FileSystem/FileSystem.h"
#include "Core/Scene.h"

BasicMaterial::BasicMaterial()
{
}

BasicMaterial::~BasicMaterial()
{
}

void BasicMaterial::SetColor(const glm::vec3& color)
{
    Scene::GetBasicShader()->Bind();
    Scene::GetBasicShader()->SetUniformVec3("u_Color", color);
    m_Color = color;
}
