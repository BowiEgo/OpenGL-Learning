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

void InstanceMesh::SetPosition(unsigned int index, glm::vec3 &position)
{
    m_Coordinates[index].Position = position;
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

void InstanceMesh::Setup()
{
    for (unsigned int i = 0; i < m_Capacity; i++)
    {
        MeshCoordinate coordinate = MeshCoordinate();
        m_Coordinates.push_back(coordinate);
    }
}
