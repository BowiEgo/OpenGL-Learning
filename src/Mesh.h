#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Vertex.h"
#include "Geometry.h"
#include "Material.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Texture2D.h"

class Geometry;

class Mesh
{
public:
    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures, std::shared_ptr<Material> material);
    ~Mesh();

    inline std::shared_ptr<Geometry> GetGeometry() { return m_Geometry; }
    inline std::shared_ptr<Material> GetMaterial() { return m_Material; }

    void Setup();
    void Draw();
private:
    std::shared_ptr<Geometry> m_Geometry;
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
};