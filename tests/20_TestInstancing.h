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

        Ref<Model> m_Model_Rock;
    };
}