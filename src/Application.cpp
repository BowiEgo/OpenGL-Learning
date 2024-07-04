#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"
#include "Core/Renderer.h"
#include "Core/Framebuffer/FramebufferNew.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Input.h"
#include "KeyCodes.h"

#include "../tests/01_TestClearColor.h"
#include "../tests/02_TestTexture2D.h"
#include "../tests/03_TestTexture2DContainer.h"
#include "../tests/04_TestTransform.h"
#include "../tests/05_TestCoordinate.h"
#include "../tests/06_TestCamera.h"
#include "../tests/07_TestBasicLighting.h"
#include "../tests/08_TestMaterial.h"
#include "../tests/09_TestLightingMaps.h"
#include "../tests/10_TestLightCasters.h"
#include "../tests/11_TestModel.h"
#include "../tests/12_TestDepth.h"
#include "../tests/13_TestStencil.h"
#include "../tests/14_TestBlend.h"
#include "../tests/15_TestFaceCulling.h"
#include "../tests/16_TestFramebuffer.h"
#include "../tests/17_TestCubemap.h"
#include "../tests/18_TestAdvancedGLSL.h"
#include "../tests/19_TestGeometryShader.h"
#include "../tests/20_TestInstancing.h"
#include "../tests/21_TestAntiAliasing.h"
#include "../tests/22_TestBlinnPhong.h"
#include "../tests/23_TestShadowMapping.h"
#include "../tests/24_TestPointShadow.h"
#include "../tests/25_TestNormalMapping.h"
#include "../tests/26_TestParallaxMapping.h"
#include "../tests/27_TestHDR.h"
#include "../tests/28_TestBloom.h"
#include "../tests/29_TestDeferredShading.h"
#include "../tests/31_TestPBR.h"
#include "../tests/32_TestDiffuseIrradiance.h"

void RenderUI()
{
    // Create a dockspace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }
    else
    {
        ImGui::Text("Docking is not enabled!");
    }
    ImGui::End();
}

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
    /* Enable Multisample Buffer for MSAA */
    // glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Learning", NULL, NULL);
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

    GLCall(glEnable(GL_MULTISAMPLE));
    // Blending
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Query the maximum number of samples supported
    GLint maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    std::cout << "Maximum number of samples supported: " << maxSamples << std::endl;
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
    testMenu->RegisterTest<test::TestLightCasters>("LightCasters");
    testMenu->RegisterTest<test::TestModel>("Model");
    testMenu->RegisterTest<test::TestDepth>("Depth");
    testMenu->RegisterTest<test::TestStencil>("Stencil");
    testMenu->RegisterTest<test::TestBlend>("Blend");
    testMenu->RegisterTest<test::TestFaceCulling>("FaceCulling");
    testMenu->RegisterTest<test::TestFramebuffer>("Framebuffer");
    testMenu->RegisterTest<test::TestCubemap>("Cubemap");
    testMenu->RegisterTest<test::TestAdvancedGLSL>("AdvancedGLSL");
    testMenu->RegisterTest<test::TestGeometryShader>("GeometryShader");
    testMenu->RegisterTest<test::TestInstancing>("Instancing");
    testMenu->RegisterTest<test::TestAntiAliasing>("AntiAliasing");
    testMenu->RegisterTest<test::TestBlinnPhong>("BlinnPhong");
    testMenu->RegisterTest<test::TestShadowMapping>("ShadowMapping");
    testMenu->RegisterTest<test::TestPointShadow>("PointShadow");
    testMenu->RegisterTest<test::TestNormalMapping>("NormalMapping");
    testMenu->RegisterTest<test::TestParallaxMapping>("ParallaxMapping");
    testMenu->RegisterTest<test::TestHDR>("HDR");
    testMenu->RegisterTest<test::TestBloom>("Bloom");
    testMenu->RegisterTest<test::TestDeferredShading>("DeferredShading");
    testMenu->RegisterTest<test::TestPBR>("PBR");
    testMenu->RegisterTest<test::TestDiffuseIrradiance>("DiffuseIrradiance");

    // test::TestClearColor test;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    // Framebuffer
    FramebufferManager::Create();

    GLint samples = 4;
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    fbSpec.Samples = std::min(samples, maxSamples);
    fbSpec.HDR = true;
    std::shared_ptr<Framebuffer> m_Framebuffer = FramebufferManager::CreateFramebuffer("viewport", fbSpec);

    FramebufferSpecification nonMultisample_fbSpec;
    nonMultisample_fbSpec.Width = 1280;
    nonMultisample_fbSpec.Height = 720;
    nonMultisample_fbSpec.Samples = 1;
    nonMultisample_fbSpec.HDR = true;
    std::shared_ptr<Framebuffer> m_Non_Multisample_Framebuffer = FramebufferManager::CreateFramebuffer("nonMultisample_viewport", nonMultisample_fbSpec);

    auto backTestMenu = [&]()
    {
        if (currentTest != testMenu)
        {
            GLCall(glDisable(GL_DEPTH_TEST));
            delete currentTest;
            currentTest = testMenu;
        }
    };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (Input::IsKeyPressed(OPL_KEY_ESCAPE))
        {
            Input::EnableCursor();
            currentTest->DisableCameraControll();
        }
        if (Input::IsKeyPressed(OPL_KEY_SPACE))
        {
            Input::DisableCursor();
            currentTest->EnableCameraControll();
        }
        if (Input::IsKeyPressed(OPL_KEY_BACKSPACE))
        {
            Input::EnableCursor();
            backTestMenu();
        }

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        /* Render here */
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderUI();

        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime);
            currentTest->ProcessInput(deltaTime);
            if (FramebufferManager::s_SMAA_Enabled)
            {
                m_Framebuffer->Bind();
            } else {
                m_Non_Multisample_Framebuffer->Bind();
            }
            currentTest->OnRender();
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            m_Non_Multisample_Framebuffer->Unbind();
            if (FramebufferManager::s_SMAA_Enabled)
            {
                GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer->GetID()));
            } else {
                GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Non_Multisample_Framebuffer->GetID()));
            }
            GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Non_Multisample_Framebuffer->GetID()));
            GLCall(glBlitFramebuffer(0, 0, m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height, 0, 0, m_Non_Multisample_Framebuffer->GetSpecification().Width, m_Non_Multisample_Framebuffer->GetSpecification().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
            GLCall(glViewport(0, 0, m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            ImGui::Begin("Test");
            if (ImGui::Button("<-"))
                backTestMenu();
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize =  ImGui::GetContentRegionAvail();
        if (test::GetViewportSize() != *((glm::vec2*)&viewportPanelSize))
        {
            m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            m_Non_Multisample_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            test::UpdateViewportSize(viewportPanelSize.x, viewportPanelSize.y);
            currentTest->OnViewPortResize();
        }
        uint32_t textureID = m_Non_Multisample_Framebuffer->GetColorAttachments()[0];
        ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ test::GetViewportSize().x, test::GetViewportSize().y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

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