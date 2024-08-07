#include "Mesh.h"

#include "Material/MaterialManager.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : m_Geometry(geometry), m_Material(material)
{
    std::vector<Vertex>* vr = geometry->GetVertex();
    m_Vertices = std::vector<Vertex>(vr->begin(), vr->end());
    std::vector<unsigned int>* vi = geometry->GetIndices();
    m_Indices = std::vector<unsigned int>(vi->begin(), vi->end());
    Setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, Ref<Material> material)
    : m_Vertices(vertices), m_Material(material)
{
    Setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::shared_ptr<Material> material)
    : m_Vertices(vertices), m_Indices(indices), m_Material(material)
{
    Setup();
}

void Mesh::AddInstanceVBO(const Ref<VertexBuffer> instanceVBO, const unsigned int size, const unsigned unitSize, const unsigned int times)
{
    m_VAO->Bind();
    instanceVBO->Bind();
    for (unsigned int i = 0; i < times; i++)
    {
        glEnableVertexAttribArray(m_VAO->GetElementCount());
        glVertexAttribPointer(m_VAO->GetElementCount(), size, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(m_VAO->GetElementCount(), 1);
        m_VAO->IncreaseElementCount();
    }
    instanceVBO->Unbind();
    m_InstanceVBOs.push_back(instanceVBO);
}

void Mesh::Setup()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    if (m_Indices.size() != 0)
        m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());
    
    m_Layout = std::make_unique<VertexBufferLayout>();
    // Vertex positions
    m_Layout->Push<Vertex>(3, 0);
    // Vertex normals
    m_Layout->Push<Vertex>(3, (unsigned int)offsetof(Vertex, Normal));
    // Vertex texture coords
    m_Layout->Push<Vertex>(2, (unsigned int)offsetof(Vertex, TexCoords));
    // Vertex tangent
    m_Layout->Push<Vertex>(3, (unsigned int)offsetof(Vertex, Tangent));
    // Vertex bitangent
    m_Layout->Push<Vertex>(3, (unsigned int)offsetof(Vertex, Bitangent));
    // Bone ids
    m_Layout->Push<Vertex>(4, (unsigned int)offsetof(Vertex, m_BoneIDs));
    // Bone weights
    m_Layout->Push<Vertex>(4, (unsigned int)offsetof(Vertex, m_Weights));

    m_VAO->AddBufferLayout(*m_VBO, *m_Layout);
}

void Mesh::SetDrawType(const DrawType &drawType)
{
    m_DrawType = drawType;
}

void Mesh::SetDrawWay(const DrawWay &drawWay, const unsigned int& instanceCount)
{
    m_DrawWay = drawWay;
    m_InstanceCount = instanceCount;
}

void Mesh::SetMaterial(Ref<Material> material)
{
    m_Material = material;
}

void Mesh::SetPosition(const float position[3])
{
    m_Position.x = position[0];
    m_Position.y = position[1];
    m_Position.z = position[2];
}

void Mesh::SetPosition(const glm::vec3 &position)
{
    m_Position = position;
}

void Mesh::SetPosition(const float x, const float y, const float z)
{
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;
}

void Mesh::SetScale(const glm::vec3 &scale)
{
    m_Scale = scale;
}

void Mesh::SetScale(const float scaleX, const float scaleY, const float scaleZ)
{
    m_Scale.x = scaleX;
    m_Scale.y = scaleY;
    m_Scale.z = scaleZ;
}

void Mesh::SetRotation(const std::pair<float, glm::vec3> rotation)
{
    m_Rotation = rotation;
}

void Mesh::SetOutline(const bool enable)
{
    Outline_Enabled = enable;
}

void Mesh::SetOutlineWidth(const float &width)
{
    m_Outline_Width = width;
}

void Mesh::SetOutlineColor(const glm::vec3 &color)
{
    m_Outline_Color = color;
}

void Mesh::Draw()
{
    Draw(GetPosition(), GetScale(), GetRotation());
}

void Mesh::Draw(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation)
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

    if (DepthTest_Enabled)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    GLCall(glStencilMask(0x00));

    if (Outline_Enabled)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF)); 
        GLCall(glStencilMask(0xFF));
    }

    Renderer renderer;
    if (m_IBO == nullptr)
        renderer.Draw(*m_VAO, m_DrawWay, m_DrawType, m_InstanceCount);
    else
        renderer.Draw(*m_VAO, *m_IBO, m_DrawWay, m_DrawType, m_InstanceCount);
}

void Mesh::DrawOutline()
{
    DrawOutline(GetPosition(), GetScale(), GetRotation());
}

void Mesh::DrawOutline(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation)
{
    if (!Outline_Enabled)
        return;

    Scene::GetMaterialManager()->GetOutlineMaterial()->Update(position, scale, rotation);
    Scene::GetMaterialManager()->GetOutlineMaterial()->BindShader();

    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE));

    GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    GLCall(glStencilMask(0x00));

    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_OutlineDrawType", Outline_DrawType);
    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_OutlineWidth", m_Outline_Width);
    Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShaderUniform("u_Color", m_Outline_Color);

    Renderer renderer;
    if (m_IBO == nullptr)
        renderer.Draw(*m_VAO, m_DrawWay, m_DrawType, m_InstanceCount);
    else
        renderer.Draw(*m_VAO, *m_IBO, m_DrawWay, m_DrawType, m_InstanceCount);

    GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
    GLCall(glStencilMask(0xFF));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDisable(GL_STENCIL_TEST));
}
