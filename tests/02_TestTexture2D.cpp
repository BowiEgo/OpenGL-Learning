#include "02_TestTexture2D.h"

#include "Core/Renderer.h"
#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestTexture2D::TestTexture2D(GLFWwindow* window)
      : Test(window),
        m_Proj(glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
    {
        float positions[] = {
            //-- positions -- texCoord --
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
             50.0f, -50.0f, 1.0f, 0.0f, // 1
             50.0f,  50.0f, 1.0f, 1.0f, // 2
            -50.0f,  50.0f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        // VertexBuffer with VAO
        m_VertexBuffer = std::make_unique<VertexBuffer >(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBufferLayout(*m_VertexBuffer, layout);

        // IndexBuffer
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        // Shader
        m_Shader = std::make_unique<Shader>("../res/shaders/Basic.shader");
        m_Shader->Bind();
        // m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        // shader.SetUniformMat4f("u_MVP", mvp);

        // Texture
        m_Texture = std::make_unique<Texture2D>("../res/textures/Avatar.png");
        m_Shader->SetUniform1i("u_Texture", 0);

        // Test unbind VertexArray, VertexBuffer, IndexBuffer and Shader
        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();
    }
    
    TestTexture2D::~TestTexture2D()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); 
            m_Shader->SetUniformMat4("u_MVP", mvp);

            renderer.Draw(*m_Shader, *m_VAO, *m_IndexBuffer);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); 
            m_Shader->SetUniformMat4("u_MVP", mvp);

            renderer.Draw(*m_Shader, *m_VAO, *m_IndexBuffer);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat("TranslationA-X", &m_TranslationA.x, 0.0f, WINDOW_WIDTH);
        ImGui::SliderFloat("TranslationA-Y", &m_TranslationA.y, 0.0f, WINDOW_HEIGHT);

        ImGui::SliderFloat("TranslationB-X", &m_TranslationB.x, 0.0f, WINDOW_WIDTH);
        ImGui::SliderFloat("TranslationB-Y", &m_TranslationB.y, 0.0f, WINDOW_HEIGHT);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}