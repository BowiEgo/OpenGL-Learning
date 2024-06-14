#include "MaterialManager.h"

#include "Core/Shader/Shader.h"
#include "FileSystem/FileSystem.h"

MaterialManager* MaterialManager::s_Instance = nullptr;

MaterialManager *MaterialManager::Create()
{
    s_Instance = new MaterialManager();

    std::string singleColorVertSrc = FileSystem::ReadFile("../res/shaders/Outline.vert");
    std::string singleColorFragSrc = FileSystem::ReadFile("../res/shaders/SingleColor.frag");

    Ref<Shader> shader = std::make_shared<Shader>(singleColorVertSrc, singleColorFragSrc, "../res/shaders/SingleColor");
    s_Instance->m_OutlineMaterial = std::make_shared<ShaderMaterial>(shader);
    return s_Instance;
}