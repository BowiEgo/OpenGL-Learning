#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#include "Camera/Camera.h"
#include "Camera/OrthographicCamera.h"
#include "DepthMapFBO.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

#include "Core/Material/BasicMaterial.h"

namespace test {
    class TestNormalMapping : public Test
    {
    public:
        TestNormalMapping(GLFWwindow* window);
        ~TestNormalMapping();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<PointLight> m_PointLight;
        glm::vec3 m_LightPos = glm::vec3(0.0f);
        Ref<Mesh> m_Mesh_lightCube;

        bool m_NormalMapping_Enabled = true;

        Ref<Mesh> m_Mesh_Wall;
        float m_Rotate_Wall;

        Ref<Model> m_Model_Nanosuit;
    };
}