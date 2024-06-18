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

#include "Core/Material/BasicMaterial.h"

namespace test {
    class TestBlinnPhong : public Test
    {
    public:
        TestBlinnPhong(GLFWwindow* window);
        ~TestBlinnPhong();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Scene* m_Scene;
        
        bool m_SMAA_Enabled;
        bool m_BlinnPhong_Enabled;

        Ref<Mesh> m_Mesh_Floor;
    };
}