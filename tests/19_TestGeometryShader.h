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

        Ref<Model> m_Model_Nanosuit, m_Normal_Visualize_Nanosuit;

        float m_ScreenSplit_X = 1.0f;
        float m_ExplodeProgress = 0.02f;
    };
}