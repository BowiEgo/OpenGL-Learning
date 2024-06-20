#pragma once

#include "pch.h"

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

#include "Camera/Camera.h"
#include "Core/Scene.h"
#include "Core/Model.h"
#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

namespace test {
    class TestDepth : public Test
    {
    public:
        TestDepth(GLFWwindow* window);
        ~TestDepth();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<DirectionalLight> m_DirectionalLight;
        std::vector<Ref<PointLight>> m_PointLights;

        bool m_IsPointLightsEnabled = true;
        std::vector<glm::vec3> m_ObjPositions = {
            glm::vec3( 0.0f,  0.0f,   0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f,  -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f,  -3.5f),
            glm::vec3(-1.7f,  3.0f,  -7.5f),
            glm::vec3( 1.3f, -2.0f,  -2.5f),
            glm::vec3( 1.5f,  2.0f,  -2.5f),
            glm::vec3( 1.5f,  0.2f,  -1.5f),
            glm::vec3(-1.3f,  1.0f,  -1.5f)  
        };
        std::vector<glm::vec3> m_PointLightPositions = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        float m_ZBuffer_Near = 0.1f, m_ZBuffer_Far = 10.0f;

        Ref<Shader> m_DepthShader;

        Ref<Model> m_ModelMichelle;
    };
}