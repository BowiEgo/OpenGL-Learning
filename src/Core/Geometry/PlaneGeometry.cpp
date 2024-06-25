#include "PlaneGeometry.h"

PlaneGeometry::PlaneGeometry()
{
    for (unsigned int i = 0; i < m_Vertex.size(); i++)
    {
        std::pair<glm::vec3, glm::vec3> tangents = CalcTangent(floor(i / 3));
        m_Vertex[i].Tangent = tangents.first;
        m_Vertex[i].Bitangent = tangents.second;
    }
}

std::pair<glm::vec3, glm::vec3> PlaneGeometry::CalcTangent(unsigned int triangleIdx)
{
    glm::vec3 edge1 = m_Vertex[1 + triangleIdx * 3].Position - m_Vertex[0 + triangleIdx * 3].Position;
    glm::vec3 edge2 = m_Vertex[2 + triangleIdx * 3].Position - m_Vertex[0 + triangleIdx * 3].Position;
    glm::vec2 deltaUV1 = m_Vertex[1 + triangleIdx * 3].TexCoords - m_Vertex[0 + triangleIdx * 3].TexCoords;
    glm::vec2 deltaUV2 = m_Vertex[2 + triangleIdx * 3].TexCoords - m_Vertex[0 + triangleIdx * 3].TexCoords;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent, bitangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = glm::normalize(bitangent);

    return {tangent, bitangent};
}
