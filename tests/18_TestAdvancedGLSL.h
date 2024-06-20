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

namespace test {
    class TestAdvancedGLSL : public Test
    {
    public:
        TestAdvancedGLSL(GLFWwindow* window);
        ~TestAdvancedGLSL();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<DirectionalLight> m_DirectionalLight;
        std::vector<Ref<PointLight>> m_PointLights;

        std::vector<Ref<Mesh>> m_Mesh_Container;

        float m_ScreenSplit_X = 0.5f;
    };
}