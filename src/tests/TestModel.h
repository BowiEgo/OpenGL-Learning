#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture2D.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"
#include "Model.h"

#include <memory>
#include <vector>

namespace test {
    class TestModel : public Test
    {
    public:
        TestModel(GLFWwindow* window);
        ~TestModel();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        std::unique_ptr<Model> m_ModelNanosuit, m_ModelBackpack, m_ModelMichelle;
        std::unique_ptr<VertexArray> m_Obj_VAO, m_LightCube_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_ObjShader, m_PointLightCubeShader;
        std::unique_ptr<Texture2D> m_DiffuseTexture, m_SpecularTexture;
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

        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<Light> m_Light;
    };
}