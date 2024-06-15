#include "UniformBuffer.h"

#include "../Renderer.h"
#include <glm/gtc/type_ptr.hpp>

unsigned int UniformBuffer::s_UniformBindingPoint = 0;

UniformBuffer::UniformBuffer(unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));

    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    m_BindingPoint = ++s_UniformBindingPoint;
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererID, 0, size));
}

void UniformBuffer::UpdateMat4(const unsigned int bindingPoint, const unsigned int offset, const unsigned int size, const glm::mat4& value) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, glm::value_ptr(value));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
