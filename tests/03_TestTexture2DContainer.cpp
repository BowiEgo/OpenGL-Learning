#include "03_TestTexture2DContainer.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestTexture2DContainer::TestTexture2DContainer(GLFWwindow* window)
      : Test(window),
        m_Proj(glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        float vertices[4 * 8] = {
            //------ positions ----------- color --------- texCoord --
              0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left-down
            200.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // right-down
            200.0f, 200.0f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // right-top
              0.0f, 200.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // left-top
        };

        // float vertices[4 * 8] = {
        //     //------ positions ----------- color --------- texCoord --
        //       0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // left-down
        //     200.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // right-down
        //     200.0f, 200.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // right-top
        //       0.0f, 200.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // left-top
        // };

        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VAO = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer >(vertices, 4 * 8 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        // Shader
        m_Shader = std::make_unique<Shader>("../res/shaders/ContainerTexture.shader");
        m_Shader->Bind();

        // Texture
        TextureOptions opts1;
        opts1.magFilter = GL_NEAREST;
        m_Texture1 = std::make_unique<Texture2D>("../res/textures/container.jpg", opts1);
        m_Shader->SetUniform1i("u_Texture1", 0);

        TextureOptions opts2;
        opts2.wrapS = GL_REPEAT;
        opts2.wrapT = GL_REPEAT;
        m_Texture2 = std::make_unique<Texture2D>("../res/textures/awesomeface.png", opts2);
        m_Shader->SetUniform1i("u_Texture2", 1);

        // Uniform
        m_Shader->SetUniform1f("u_Mix", m_Mix);
    }

    TestTexture2DContainer::~TestTexture2DContainer()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
    }

    void TestTexture2DContainer::OnUpdate(float deltaTime)
    {
        
    }

    void TestTexture2DContainer::OnRender()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture1->Bind();
        m_Texture2->Bind(1);

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 150.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); 
            m_Shader->SetUniformMat4("u_MVP", mvp);
            m_Shader->SetUniform1f("u_Mix", m_Mix);

            renderer.Draw(*m_Shader, *m_VAO, *m_IndexBuffer);
        }
    }

    void TestTexture2DContainer::OnImGuiRender()
    {
        ImGui::SliderFloat("Mix", &m_Mix, 0.0f, 1.0f);
    }
}