#include "config.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "Failed to initialize GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f, // 0
        50.0f, -50.0f, 1.0f, 0.0f, // 1
        50.0f, 50.0f, 1.0f, 1.0f, // 2
        -50.0f, 50.0f, 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Blending
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // VertexBuffer with VAO
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // IndexBuffer
    IndexBuffer ib(indices, 6);

    // Projection Matrix
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    // Shader
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    // shader.SetUniformMat4f("u_MVP", mvp);

    // Texture
    Texture texture("res/textures/Avatar.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    // Test unbind VertexArray, VertexBuffer, IndexBuffer and Shader
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    // Renderer
    Renderer renderer;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // ImGui state
    bool show_demo_window = true;

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    // Animation variables
    float r = 0.0f;
    float increment = 1.0f;
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.Bind(); 
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.Bind(); 
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        }
        
        // Animation
        if (r > 1.0f)
            increment = -1.0f;
        else if (r < 0.0f)
            increment = 1.0f;
        
        r += increment * deltaTime.count();

        // ImGui window
        // ImGui::ShowDemoWindow(&show_demo_window);
        {
            static float f = 0.0f;

            ImGui::SliderFloat("TranslationA-X", &translationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat("TranslationA-Y", &translationA.y, 0.0f, 540.0f);

            ImGui::SliderFloat("TranslationB-X", &translationB.x, 0.0f, 960.0f);
            ImGui::SliderFloat("TranslationB-Y", &translationB.y, 0.0f, 540.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        // ImGui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}