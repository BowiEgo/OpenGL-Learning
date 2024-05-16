#include "config.h"

#include "src/Renderer.h"

#include "src/VertexBuffer.h"
#include "src/VertexBufferLayout.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"
#include "src/Shader.h"
#include "src/Texture.h"

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f  // 3
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

    // Shader
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

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

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        renderer.Draw(va, ib, shader);

        // Animation
        if (r > 1.0f)
            increment = -1.0f;
        else if (r < 0.0f)
            increment = 1.0f;
        
        r += increment * deltaTime.count();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}