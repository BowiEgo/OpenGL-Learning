#include "Mesh.h"

#include "VertexBufferLayout.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
    m_VAO = std::make_shared<VertexArray>();
    m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
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

Mesh::~Mesh()
{
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
    renderer.Draw(*shader, *m_VAO, *m_IBO);
}