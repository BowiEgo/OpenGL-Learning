#include "TestCamera.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "KeyCodes.h"

namespace test {
    TestCamera::TestCamera(GLFWwindow* window)
      : Test(window),
        m_Proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_CameraRight(glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_CameraFront)))
    {
        GLCall(glEnable(GL_DEPTH_TEST));

        GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // A 0
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // B 1
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // C 2
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // D 3
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // E 4
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // F 5
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // G 6
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // H 7
    
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // D 8
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // A 9
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // E 10
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // H 11
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // B 12
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   // C 13
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // G 14
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   // F 15
    
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // A 16
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // B 17
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,   // F 18
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // E 19
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // C 20
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // D 21
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // H 22
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // G 23
        };
        // index data
        GLuint indices[] = {
            // front and back
             0,  3,  2,
             2,  1,  0,
             4,  5,  6,
             6,  7,  4,
            // left and right
            11,  8,  9,
             9, 10, 11,
            12, 13, 14,
            14, 15, 12,
            // bottom and top
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
        };
        
        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, 36);

        // Shader
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/ContainerTexture.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/ContainerTexture.frag");
        m_Shader = std::make_unique<Shader>(vertexSrc, fragSrc);
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

        // CubePositions
        m_CubePositions = {
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

        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    TestCamera::~TestCamera()
    {
    }

    void TestCamera::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestCamera::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        m_Texture1->Bind();
        m_Texture2->Bind(1);

        {
            m_Proj = glm::perspective(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.f);
            m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
            glm::mat4 vp = m_Proj * m_View;
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, m_CubePositions[i]);
                float angle = 20.0f * (i + 1);
                if (i % 3)
                    angle = (float)glfwGetTime() * angle;

                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                
                glm::mat4 mvp = vp * model;
                m_Shader->SetUniformMat4("u_MVP", mvp);

                renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
            }
        }
    }

    void TestCamera::OnImGuiRender()
    {
        ImGui::SliderFloat("FOV", &m_FOV, 0.0f, 180.0f);
        ImGui::SliderFloat("AspectRatio", &m_AspectRatio, 0.0f, 10.0f);
        ImGui::PushItemWidth(80);
        ImGui::Text("CameraTarget:");
        ImGui::SliderFloat("X", &m_CameraTarget.x, -5.0f, 5.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("Y", &m_CameraTarget.y, -5.0f, 5.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("Z", &m_CameraTarget.z, -5.0f, 5.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestCamera::ProcessInput(float deltaTime)
    {
        float cameraSpeed = 2.5f * deltaTime;
        if (Input::IsKeyPressed(OPL_KEY_W))
            m_CameraPos += cameraSpeed * m_CameraFront;
        if (Input::IsKeyPressed(OPL_KEY_S))
            m_CameraPos -= cameraSpeed * m_CameraFront;
        if (Input::IsKeyPressed(OPL_KEY_A))
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        if (Input::IsKeyPressed(OPL_KEY_D))
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        
        float mouseX = Input::GetMouseX();
        float mouseY = Input::GetMouseY();
        float offsetX = mouseX - m_LastMouseX;
        float offsetY = mouseY - m_LastMouseY;
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;

        float sensitivity = 0.05f;
        offsetX *= sensitivity;
        offsetY *= sensitivity;

        m_Yaw += offsetX;
        m_Pitch -= offsetY;
        CORE_INFO("{0}, {1}", mouseX, mouseY);
        CORE_INFO("{0}, {1}", m_Yaw, m_Pitch);

        if(m_Pitch > 89.0f)
            m_Pitch =  89.0f;
        if(m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        glm::vec3 front(0.0f, 0.0f, -1.0f);
        front.x = sin(glm::radians(m_Yaw));
        front.y = sin(glm::radians(m_Pitch));
        // front.z = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_CameraFront = glm::normalize(front);

        float yScrollOffset = Input::GetMouseScroll();

        if(m_FOV >= 1.0f && m_FOV <= 45.0f)
            m_FOV -= yScrollOffset * 1.0;
        if(m_FOV <= 1.0f)
            m_FOV = 1.0f;
        if(m_FOV >= 45.0f)
            m_FOV = 45.0f;
    }
}