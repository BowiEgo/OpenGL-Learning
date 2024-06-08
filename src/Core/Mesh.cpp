#include "Mesh.h"

#include "Core/VertexBufferLayout.h"

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

Mesh::~Mesh()
{
}

void Mesh::Setup()
{
    m_VAO = std::make_shared<VertexArray>();
    m_VBO = std::make_shared<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    if (m_Indices.size() != 0)
        m_IBO = std::make_shared<IndexBuffer>(&m_Indices[0], m_Indices.size());

    VertexBufferLayout layout;

    // Vertex Positions
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
    m_Position[0] = position[0];
    m_Position[1] = position[1];
    m_Position[2] = position[2];
}

void Mesh::SetPosition(glm::vec3 &position)
{
    m_Position[0] = position.x;
    m_Position[1] = position.y;
    m_Position[2] = position.z;
}

void Mesh::SetPosition(float x, float y, float z)
{
    m_Position[0] = x;
    m_Position[1] = y;
    m_Position[2] = z;
}

void Mesh::SetScale(float scaleX, float scaleY, float scaleZ)
{
    m_Scale[0] = scaleX;
    m_Scale[1] = scaleY;
    m_Scale[2] = scaleZ;
}

void Mesh::SetRotation(std::pair<float, glm::vec3> &rotation)
{
    m_Rotation = rotation;
}

void Mesh::Draw(Shader* shader)
{
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

        shader->SetUniform1i(("u_" + type + number).c_str(), i);
    }

    Renderer renderer;
    if (m_IBO == nullptr)
        renderer.Draw(*shader, *m_VAO);
    else
        renderer.Draw(*shader, *m_VAO, *m_IBO);
}