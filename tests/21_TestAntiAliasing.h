#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#include "Camera/Camera.h"
#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

#include "Core/Material/BasicMaterial.h"

namespace test {
    class TestAntiAliasing : public Test
    {
    public:
        TestAntiAliasing(GLFWwindow* window);
        ~TestAntiAliasing();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        
        bool m_SMAA_Enabled;

        Ref<BasicMaterial> m_Container_Material;
        glm::vec3 m_Container_Color = glm::vec3(0.5f, 0.5f, 1.0f);
    };
}