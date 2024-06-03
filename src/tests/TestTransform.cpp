#include "TestTransform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test {
    TestTransform::TestTransform(GLFWwindow* window)
      : Test(window),
        m_Proj(glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_Transform(glm::mat4(1.0f))
    {
        float vertices[4 * 8] = {
            //-- positions ----------- color --------- texCoord --
            -100.0f, -100.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left-down
             100.0f, -100.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right-down
             100.0f,  100.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right-top
            -100.0f,  100.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left-top
        };

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
        m_Texture1 = std::make_unique<Texture>("../res/textures/container.jpg", opts1);
        m_Shader->SetUniform1i("u_Texture1", 0);

        TextureOptions opts2;
        m_Texture2 = std::make_unique<Texture>("../res/textures/awesomeface.png");
        m_Shader->SetUniform1i("u_Texture2", 1);

        // Uniform
        m_Shader->SetUniform1f("u_Mix", m_Mix);
    }

    TestTransform::~TestTransform()
    {
    }

    void TestTransform::OnUpdate(float deltaTime)
    {
    }

    void TestTransform::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture1->Bind();
        m_Texture2->Bind(1);

        {
            m_Transform = glm::mat4(1.0f);
            m_Transform = glm::scale(m_Transform, glm::vec3(0.5, 0.5, 0.5));
            m_Transform = glm::rotate(m_Transform, (float)(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 model = m_Transform;
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); 
            m_Shader->SetUniformMat4("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            m_Transform2 = glm::mat4(1.0f);
            m_Transform2 = glm::translate(m_Transform2, glm::vec3(-250.0f, 150.0f, 0.0f));
            float scaleAmount = sin(glfwGetTime());
            m_Transform2 = glm::scale(m_Transform2, glm::vec3(scaleAmount));
            glm::mat4 model = m_Transform2;
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); 
            m_Shader->SetUniformMat4("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestTransform::OnImGuiRender()
    {
    }
}