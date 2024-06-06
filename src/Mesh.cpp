#include "Mesh.h"

#include "VertexBufferLayout.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
    m_VAO = std::make_shared<VertexArray>();
    m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
    m_IBO = std::make_shared<IndexBuffer>(&m_Indices[0], m_Indices.size());

    VertexBufferLayout layout;

    layout.Push<Vertex>(3, 0);
    layout.Push<Vertex>(3, (unsigned int)offsetof(Vertex, Normal));
    layout.Push<Vertex>(2, (unsigned int)offsetof(Vertex, TexCoords));

    m_VAO->AddBuffer(*m_VBO, layout);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader* shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for(unsigned int i = 0; i < m_Textures.size(); i++)
    {
        std::shared_ptr<Texture2D>& texture = m_Textures[i];
        texture->Bind(i);
        std::string number;
        std::string type = texture->GetType();
        if(type == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(type == "texture_specular")
            number = std::to_string(specularNr++);

        shader->SetUniform1i(("material." + type + number).c_str(), i);
    }
    glActiveTexture(GL_TEXTURE0);

    Renderer renderer;
    renderer.Draw(*shader, *m_VAO, *m_IBO);
}