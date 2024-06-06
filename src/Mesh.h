#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Texture2D.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
    ~Mesh();

    void Draw(Shader* shader);
private:
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
};