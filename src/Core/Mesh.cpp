#include "Mesh.h"

#include "Core/VertexBufferLayout.h"
#include "Material/MaterialManager.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : m_Geometry(geometry), m_Material(material)
{
    std::array<Vertex, 36>* vr = geometry->GetVertex();
    m_Vertices = std::vector<Vertex>(vr->begin(), vr->end());
    Setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, std::shared_ptr<Material> material)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_Material(material)
{
    Setup();
}

void Mesh::Setup()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    if (m_Indices.size() != 0)
        m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());

    VertexBufferLayout layout;

    // Vertex positions
    layout.Push<Vertex>(3, 0);
    // Vertex normals
    layout.Push<Vertex>(3, (unsigned int)offsetof(Vertex, Normal));
    // Vertex texture coords
    layout.Push<Vertex>(2, (unsigned int)offsetof(Vertex, TexCoords));
    // Vertex tangent
    layout.Push<Vertex>(3, (unsigned int)offsetof(Vertex, Tangent));
    // Vertex bitangent
    layout.Push<Vertex>(3, (unsigned int)offsetof(Vertex, Bitangent));
    // Bone ids
    layout.Push<Vertex>(4, (unsigned int)offsetof(Vertex, m_BoneIDs));
    // Bone weights
    layout.Push<Vertex>(4, (unsigned int)offsetof(Vertex, m_Weights));

    m_VAO->AddBuffer(*m_VBO, layout);
}

void Mesh::SetMaterial(Ref<Material> material)
{
    m_Material = material;
}

void Mesh::SetPosition(float position[3])
{
    m_Position.x = position[0];
    m_Position.y = position[1];
    m_Position.z = position[2];
}

void Mesh::SetPosition(glm::vec3 &position)
{
    m_Position = position;
}

void Mesh::SetPosition(float x, float y, float z)
{
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;
}

void Mesh::SetScale(glm::vec3 &scale)
{
    m_Scale = scale;
}

void Mesh::SetScale(float scaleX, float scaleY, float scaleZ)
{
    m_Scale.x = scaleX;
    m_Scale.y = scaleY;
    m_Scale.z = scaleZ;
}

void Mesh::SetRotation(std::pair<float, glm::vec3> rotation)
{
    m_Rotation = rotation;
}

void Mesh::SetOutline(bool enable)
{
    Outline_Enabled = enable;
}

void Mesh::SetOutlineWidth(float &width)
{
    m_Outline_Width = width;
}

void Mesh::SetOutlineColor(glm::vec3 &color)
{
    m_Outline_Color = color;
}

void Mesh::Draw(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation)
{
    GetMaterial()->Update(position, scale, rotation);

    switch (Cull_Face)
    {
    case CULL_FACE_NONE:
        GLCall(glDisable(GL_CULL_FACE));
        break;
    case CULL_FACE_BACK:
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_BACK));
        break;
    case CULL_FACE_FRONT:
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        break;
    case CULL_FACE_FRONT_AND_BACK:
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT_AND_BACK));
        break;
    }

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for(unsigned int i = 0; i < m_Textures.size(); i++)
    {
        std::shared_ptr<Texture2D>& texture = m_Textures[i];
        texture->Bind(i);
        std::string number;
        std::string type = texture->GetType();
        if(type == "Texture_Diffuse")
            number = std::to_string(diffuseNr++);
        else if(type == "Texture_Specular")
            number = std::to_string(specularNr++);
        else if(type == "Texture_Normal")
            number = std::to_string(normalNr++);
        else if(type == "Texture_Height")
            number = std::to_string(heightNr++);

        GetMaterial()->UpdateShaderUniform(("u_" + type + number).c_str(), i);
    }

    GLCall(glEnable(GL_DEPTH_TEST));

    GLCall(glStencilMask(0x00));

    if (Outline_Enabled)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF)); 
        GLCall(glStencilMask(0xFF));
    }

    Renderer renderer;
    if (m_IBO == nullptr)
        renderer.Draw(*m_VAO);
    else
        renderer.Draw(*m_VAO, *m_IBO);
}

void Mesh::DrawOutline(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation)
{
    if (!Outline_Enabled)
        return;

    Scene::GetMaterialManager()->GetOutlineMaterial()->Update(position, scale, rotation);

    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE));

    GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    GLCall(glStencilMask(0x00));

    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_OutlineDrawType", Outline_DrawType);
    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_OutlineWidth", m_Outline_Width);
    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_OutlineColor", m_Outline_Color);

    Renderer renderer;
    if (m_IBO == nullptr)
        renderer.Draw(*m_VAO);
    else
        renderer.Draw(*m_VAO, *m_IBO);

    GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    GLCall(glStencilMask(0xFF));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDisable(GL_STENCIL_TEST));
}
