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
    class TestPointShadow : public Test
    {
    public:
        TestPointShadow(GLFWwindow* window);
        ~TestPointShadow();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<PerspectiveCamera> m_Viewport_Camera;
        Ref<PerspectiveCamera> m_DepthMap_Camera;

        Ref<PointLight> m_PointLight;

        Ref<ShaderMaterial> m_Material_DepthMap;
        Ref<StandardMaterial> m_Material_Wood, m_Material_Container;

        Ref<Mesh> m_Mesh_Room;
        std::vector<Ref<Mesh>> m_Mesh_Boxes;
        Ref<Model> m_Model_Michelle;

        Ref<DepthMapFBO> m_DepthMapFBO;
        Ref<Mesh> m_Mesh_Screen;

        Ref<TextureCubemap> m_Texture_ShadowMap;
        std::vector<glm::mat4> m_ShadowTransforms;
        float m_Near = 1.0f;
        float m_Far = 25.0f;
        glm::vec3 m_LightPos = glm::vec3(0.0f);

        const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        GLuint depthMapFBO;
        GLuint depthCubemap;

        Ref<Shader> shader_depthMap;
    };
}