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

#define OUTLINE_DRAWTYPE_SCALE 0;
#define OUTLINE_DRAWTYPE_NORMAL 1;

class Geometry;

class Mesh
{
public:
    Mesh(Ref<Geometry> geometry, Ref<Material> material);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture2D>> textures, Ref<Material> material);
    ~Mesh();

    inline Ref<Geometry> GetGeometry() { return m_Geometry; }
    inline Ref<Material> GetMaterial() { return m_Material; }
    inline float* GetPosition() { return m_Position; }
    inline float* GetScale() { return m_Scale; }
    inline std::pair<float, glm::vec3>* GetRotation() { return &m_Rotation; }

    void Setup();
    void SetMaterial(Ref<Material> material);
    void SetPosition(float position[3]);
    void SetPosition(glm::vec3& position);
    void SetPosition(float x, float y, float z);
    void SetScale(float scaleX, float scaleY, float scaleZ);
    void SetRotation(std::pair<float, glm::vec3>& rotation);
    void SetOutline(bool enable);
    void SetOutlineWidth(float& width);
    void Draw();
    void DrawOutline();
public:
    bool Outline_Enabled = false;
    bool Outline_DrawType = OUTLINE_DRAWTYPE_SCALE;
private:
    Ref<Geometry> m_Geometry;
    Ref<Material> m_Material;
    std::vector<Ref<Texture2D>> m_Textures;

    Scope<VertexArray> m_VAO;
    Scope<VertexBuffer> m_VBO;
    Scope<IndexBuffer> m_IBO;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    float m_Position[3] = { 0.0f, 0.0f, 0.0f };
    float m_Scale[3] = { 1.0f, 1.0f, 1.0f };
    std::pair<float, glm::vec3> m_Rotation = { 0.0, glm::vec3(0.0, 0.0, 1.0) };

    float m_Outline_Width = 1.0f;
};