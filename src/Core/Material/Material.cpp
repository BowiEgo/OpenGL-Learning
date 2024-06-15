#include "Material.h"

#include "Log.h"
#include <glad/glad.h>

void Material::Update(const glm::vec3& position, const glm::vec3& scale, std::pair<float, const glm::vec3>* rotation)
{
    if (Wireframe_Enabled)
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    UpdateShader(position, scale, rotation);
}
