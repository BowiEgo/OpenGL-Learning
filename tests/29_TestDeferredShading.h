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

#include "PostprocessingGBuffer.h"

namespace test {
    class TestDeferredShading : public Test
    {
    public:
        TestDeferredShading(GLFWwindow* window);
        ~TestDeferredShading();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::vector<Ref<PointLight>> m_PointLights;
        std::vector<Ref<Mesh>> m_Mesh_LightCubes;
        std::vector<glm::vec3> m_Model_Positions = {
            glm::vec3(-3.0, -3.0, -3.0),
            glm::vec3( 0.0, -3.0, -3.0),
            glm::vec3( 3.0, -3.0, -3.0),
            glm::vec3(-3.0, -3.0,  0.0),
            glm::vec3( 0.0, -3.0,  0.0),
            glm::vec3( 3.0, -3.0,  0.0),
            glm::vec3(-3.0, -3.0,  3.0),
            glm::vec3( 0.0, -3.0,  3.0),
            glm::vec3( 3.0, -3.0,  3.0),
        };
        Ref<Model> m_Model_Nanosuit;
        Ref<ShaderMaterial> m_Model_Material;

        glm::mat4 m_ModelMatrices[9];
        Ref<VertexBuffer> m_InstanceVBO_Nanosuit;

        Ref<PostprocessingGBuffer> m_PostprocessingGBuffer;
    };
}