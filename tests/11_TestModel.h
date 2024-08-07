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
#include "Core/Light/SpotLight.h"

namespace test {
    class TestModel : public Test
    {
    public:
        TestModel(GLFWwindow* window);
        ~TestModel();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::shared_ptr<DirectionalLight> m_DirectionalLight;
        std::vector<std::shared_ptr<PointLight>> m_PointLights;
        std::shared_ptr<SpotLight> m_SpotLight;
        std::shared_ptr<Model> m_ModelNanosuit, m_ModelBackpack, m_ModelMichelle;

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
    };
}