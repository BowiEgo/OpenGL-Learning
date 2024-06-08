#pragma once

#include "pch.h"

#include <glm/glm.hpp>

#include "Vertex.h"
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "Material/ShaderMaterial.h"

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
    inline float* GetPosition() { return m_Position; }
    inline float* GetScale() { return m_Scale; }
    inline std::pair<float, glm::vec3> GetRotation() { return m_Rotation; }

    void Setup();
    void SetMaterial(Ref<Material> material);
    void SetPosition(float position[3]);
    void SetPosition(glm::vec3& position);
    void SetPosition(float x, float y, float z);
    void SetScale(float scaleX, float scaleY, float scaleZ);
    void SetRotation(std::pair<float, glm::vec3>& rotation);
    void Draw(Shader* shader);
private:
    std::shared_ptr<Geometry> m_Geometry;
    std::shared_ptr<Material> m_Material;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    float m_Position[3] = { 0.0f, 0.0f, 0.0f };
    float m_Scale[3] = { 1.0f, 1.0f, 1.0f };
    std::pair<float, glm::vec3> m_Rotation = { 0.0, glm::vec3(0.0, 0.0, 1.0) };
};