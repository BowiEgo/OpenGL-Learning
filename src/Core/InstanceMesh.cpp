#include "InstanceMesh.h"

InstanceMesh::InstanceMesh(Ref<Geometry> geometry, Ref<Material> material, unsigned int capacity)
    : Mesh(geometry, material), m_Capacity(capacity)
{
    Setup();
}

InstanceMesh::InstanceMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture2D>> textures, Ref<Material> material, unsigned int capacity)
    : Mesh(vertices, indices, textures, material), m_Capacity(capacity)
{
    Setup();
}

void InstanceMesh::Setup()
{
    for (unsigned int i = 0; i < m_Capacity; i++)
    {
        MeshCoordinate coordinate = MeshCoordinate();
        m_Coordinates.push_back(coordinate);
    }
}

void InstanceMesh::SetPosition(unsigned int index, glm::vec3 &position)
{
    m_Coordinates[index].Position = position;
}

void InstanceMesh::SetScale(unsigned int index, glm::vec3 &scale)
{
    m_Coordinates[index].Scale = scale;
}

void InstanceMesh::SetRotation(unsigned int index, std::pair<float, glm::vec3> &rotation)
{
    m_Coordinates[index].Rotation = rotation;
}

void InstanceMesh::Draw()
{
    for (auto coordinate : m_Coordinates)
    {
        Mesh::Draw(coordinate.Position, coordinate.Scale, &coordinate.Rotation);
    }
}

void InstanceMesh::Draw(MeshCoordinate* coordinate)
{
    Mesh::Draw(coordinate->Position, coordinate->Scale, &coordinate->Rotation);
}

void InstanceMesh::DrawOutline()
{
    for (auto coordinate : m_Coordinates)
    {
        Mesh::DrawOutline(coordinate.Position, coordinate.Scale, &coordinate.Rotation);
    }
}

void InstanceMesh::DrawOutline(MeshCoordinate *coordinate)
{
    Mesh::DrawOutline(coordinate->Position, coordinate->Scale, &coordinate->Rotation);
}
