
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"
#include "Renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Input.h"
#include "KeyCodes.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture2DContainer.h"
#include "tests/TestTransform.h"
#include "tests/TestCoordinate.h"
#include "tests/TestCamera.h"
#include "tests/TestBasicLighting.h"
#include "tests/TestMaterial.h"
#include "tests/TestLightingMaps.h"

int main(void)
{
    Log::Init();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL Learning", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if(!status)
        std::cout << "Failed to initialize GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Input
    Input::Create(window);

    // Blending
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Renderer
    Renderer renderer;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 410");

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest, window);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("Basic Texture 2D");
    testMenu->RegisterTest<test::TestTexture2DContainer>("Texture 2D Container");
    testMenu->RegisterTest<test::TestTransform>("Transform");
    testMenu->RegisterTest<test::TestCoordinate>("Coordinate");
    testMenu->RegisterTest<test::TestCamera>("Camera");
    testMenu->RegisterTest<test::TestBasicLighting>("BasicLighting");
    testMenu->RegisterTest<test::TestMaterial>("Material");
    testMenu->RegisterTest<test::TestLightingMaps>("LightingMaps");

    // test::TestClearColor test;

    auto backTestMenu = [&]()
    {
        if (currentTest != testMenu)
        {
            GLCall(glDisable(GL_DEPTH_TEST));
            delete currentTest;
            currentTest = testMenu;
        }
    };

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (Input::IsKeyPressed(OPL_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);
        if (Input::IsKeyPressed(OPL_KEY_BACKSPACE))
            backTestMenu();


        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (ImGui::Button("<-"))
                backTestMenu();
            currentTest->OnImGuiRender();
            ImGui::End();
        }
       
        // ImGui rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}