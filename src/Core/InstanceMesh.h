#pragma once

#include "pch.h"
#include "Mesh.h"

struct MeshCoordinate
{
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    std::pair<float, glm::vec3> Rotation = { 0.0, glm::vec3(0.0, 0.0, 1.0) };
};

using MeshCoordinatePair = std::pair<Mesh*, MeshCoordinate>;

class InstanceMesh : public Mesh
{
public:
    InstanceMesh(Ref<Geometry> geometry, Ref<Material> material, unsigned int capacity);
    InstanceMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Ref<Material> material, unsigned int capacity);
    ~InstanceMesh() {};

    inline std::string* GetMeshType() { return &m_MeshType; }
    inline std::vector<MeshCoordinate>* GetCoordinates() { return &m_Coordinates; }

    void AddCoordinate(MeshCoordinate* coordinate);
    void SetPosition(unsigned int index, glm::vec3& position);
    void SetScale(unsigned int index, glm::vec3& scale);
    void SetRotation(unsigned int index, std::pair<float, glm::vec3>& rotation);

    void Draw();
    void Draw(MeshCoordinate* coordinate);

    void DrawOutline();
    void DrawOutline(MeshCoordinate* coordinate);
private:
    void Setup();
private:
    unsigned int m_Capacity;
    std::vector<MeshCoordinate> m_Coordinates;

    std::string m_MeshType = "instance";
};