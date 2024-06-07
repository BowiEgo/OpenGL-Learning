#include "BoxGeometry.h"

BoxGeometry::BoxGeometry()
{
    m_VAO = std::make_shared<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Vertices, sizeof(m_Vertices));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}

BoxGeometry::~BoxGeometry()
{
}
