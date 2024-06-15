#pragma once

#include "pch.h"

#include <glm/glm.hpp>

class UniformBuffer
{
public:
    UniformBuffer(unsigned int size);
   ~UniformBuffer() {};

   unsigned int GetID() const { return m_RendererID; };
   unsigned int GetBindingPoint() const { return m_BindingPoint; };

   void UpdateMat4(const unsigned int bindingPoint, const unsigned int offset, const unsigned int size, const glm::mat4& value) const;
private:
    static unsigned int s_UniformBindingPoint;
    unsigned int m_RendererID;
    unsigned int m_BindingPoint;
};