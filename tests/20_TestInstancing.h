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
    class TestInstancing : public Test
    {
    public:
        TestInstancing(GLFWwindow* window);
        ~TestInstancing();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Scene* m_Scene;

        Ref<Shader> m_Shader;
        unsigned int m_QuadVAO;

        bool m_Instancing_Enabled = true;
        Ref<Model> m_Model_Rock;
        unsigned int m_Amount = 100000;

        std::vector<glm::vec3> m_Translates, m_Scales;
        std::vector<std::pair<float, glm::vec3>> m_Rotates;

        glm::mat4 m_ModelMatrices[100000];
        Ref<VertexBuffer> m_InstanceVBO_Rock;
    };
}