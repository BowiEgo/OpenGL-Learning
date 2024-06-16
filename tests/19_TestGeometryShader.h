#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#include "Camera.h"
#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

namespace test {
    class TestGeometryShader : public Test
    {
    public:
        TestGeometryShader(GLFWwindow* window);
        ~TestGeometryShader();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Scene* m_Scene;

        bool m_Normal_Visualize_Enabled = true;
        glm::vec3 m_NormalColor = glm::vec3(1.0f, 1.0f, 0.0f);
        Ref<Mesh> m_Mesh_container_normal;
        Ref<Model> m_Model_Nanosuit, m_Normal_Visualize_Nanosuit;

        float m_ExplodeProgress = 0.02f;
    };
}