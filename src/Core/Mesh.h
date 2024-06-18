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

enum CullFaceOption {
    CULL_FACE_NONE,
    CULL_FACE_BACK,
    CULL_FACE_FRONT,
    CULL_FACE_FRONT_AND_BACK
};

class Geometry;
class VertexBufferElement;

class Mesh
{
public:
    Mesh(Ref<Geometry> geometry, Ref<Material> material);
    Mesh(std::vector<Vertex> vertices, Ref<Material> material);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Ref<Material> material);
    virtual ~Mesh() = default;

    inline VertexArray* GetVAO() { return m_VAO.get(); }
    inline std::string* GetMeshType() { return &m_MeshType; }
    inline Ref<Geometry> GetGeometry() { return m_Geometry; }
    inline Ref<Material> GetMaterial() { return m_Material; }
    inline glm::vec3& GetPosition() { return m_Position; }
    inline glm::vec3& GetScale() { return m_Scale; }
    inline std::pair<float, glm::vec3>* GetRotation() { return &m_Rotation; }

    void AddInstanceVBO(const Ref<VertexBuffer> instanceVBO, const unsigned int size, const unsigned unitSize, const unsigned int times = 1);

    void Setup();
    void SetDrawType(const DrawType& drawType);
    void SetDrawWay(const DrawWay& drawWay, const unsigned int& instanceCount);
    void SetMaterial(const Ref<Material> material);
    void SetPosition(const float position[3]);
    void SetPosition(const glm::vec3& position);
    void SetPosition(const float x, const float y, const float z);
    void SetScale(const glm::vec3& scale);
    void SetScale(const float scaleX, const float scaleY, const float scaleZ);
    void SetRotation(const std::pair<float, glm::vec3> rotation);
    void SetOutline(const bool enable);
    void SetOutlineWidth(const float& width);
    void SetOutlineColor(const glm::vec3& color);

    void Draw();
    void Draw(const glm::vec3& position, const glm::vec3& scale, const std::pair<float,glm::vec3>* rotation);
    void DrawOutline();
    void DrawOutline(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation);
public:
    bool Is_Transparent = false;
    bool Outline_Enabled = false;
    bool Outline_DrawType = OUTLINE_DRAWTYPE_SCALE;
    CullFaceOption Cull_Face = CULL_FACE_NONE;
private:
    std::string m_MeshType = "normal";
    DrawType m_DrawType = DrawType::Traingles;
    DrawWay m_DrawWay = DrawWay::None;
    unsigned int m_InstanceCount = 0;
protected:
    Ref<Geometry> m_Geometry;
    Ref<Material> m_Material;

    Scope<VertexArray> m_VAO;
    Scope<VertexBuffer> m_VBO;
    std::vector<Ref<VertexBuffer>> m_InstanceVBOs;
    Scope<IndexBuffer> m_IBO;
    Scope<VertexBufferLayout> m_Layout;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    std::pair<float, glm::vec3> m_Rotation = { 0.0, glm::vec3(0.0, 0.0, 1.0) };

    float m_Outline_Width = 1.0f;
    glm::vec3 m_Outline_Color = glm::vec3(0.7647f, 0.9294f, 0.9137f);
};