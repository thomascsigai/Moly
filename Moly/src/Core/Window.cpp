#include <Window.h>
#include <TimeManipulation.h>
#include <InputProcessing.h>

namespace Moly
{
	static uint8_t s_GLFWWindowCount = 0;
    // Initializing Time Manipulation
    float TimeManipulation::DeltaTime = 0.0f;
    float TimeManipulation::LastFrame = 0.0f;
    float TimeManipulation::GameTime = 0.0f;

    float rectangleVertices[] =
    {
        // Coords    // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

	static void GLFWErrorCallback(int error, const char* description)
	{
		ML_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Window::Window(const WindowProps& props) : windowData(props), framebufferShader(nullptr)
    {
        Init(props);
    }

	void Window::Init(const WindowProps& props)
	{
		windowData.Title = props.Title;
		windowData.Width = props.Width;
		windowData.Height = props.Height;

		ML_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        stbi_set_flip_vertically_on_load(true);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
		}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

		windowGLFW = glfwCreateWindow((int)props.Width, (int)props.Height, windowData.Title.c_str(), nullptr, nullptr);

		if (!windowGLFW)
		{
			ML_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		++s_GLFWWindowCount;

        glfwMakeContextCurrent(windowGLFW);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            ML_CORE_ERROR("Failed to initialize GLAD");
            return;
        }

        SetVSync(windowData.VSync);

        glViewport(0, 0, windowData.Width, windowData.Height);
        glfwSetFramebufferSizeCallback(windowGLFW, framebuffer_size_callback);
		glfwSetKeyCallback(windowGLFW, InputProcessing::ProcessInput);
        glfwSetWindowUserPointer(windowGLFW, &windowData);
        glfwSetWindowPos(windowGLFW, 100, 100);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
        glFrontFace(GL_CW);


        InitImGui();
        ML_CORE_INFO("ImGui Initialized");

        ML_CORE_INFO("Window {0} created", props.Title, props.Width, props.Height);
        
        UseFrameBuffer();
	}

    void Window::UseFrameBuffer()
    {
        framebufferShader = new Shader("resources/shaders/framebuffer.vert", "resources/shaders/framebuffer.frag");
        glUniform1i(glGetUniformLocation(framebufferShader->ID, "screenTexture"), 0);

        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Create Frame Buffer Object
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create Framebuffer Texture
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowData.Width, windowData.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Create Render Buffer Object
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowData.Width, windowData.Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


        // Error checking framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            ML_CORE_ERROR("Framebuffer error: {0}\n", fboStatus);

    }

    void Window::InitImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        
        float fontSize = 18.0f;// *2.0f;
        io.Fonts->AddFontFromFileTTF("resources/fonts/opensans/OpenSans-Bold.ttf", fontSize);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/opensans/OpenSans-Regular.ttf", fontSize);

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(windowGLFW, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    bool Window::Should_close()
    {
        return glfwWindowShouldClose(windowGLFW);
    }

    void Window::Clear()
    {
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

        // Démarrer un nouveau frame ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // Pas de padding
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Fond transparent

        // Configuration de la fenêtre DockSpace pour qu'elle s'étende complètement
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGui::Begin("DockSpace Demo", NULL, window_flags);
        
        // Créer le DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        // Terminer la fenêtre DockSpace
        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void Window::Update()
    {
        TimeManipulation::Update(static_cast<float>(glfwGetTime()));
        
        glfwSwapBuffers(windowGLFW);
        glfwPollEvents();
    }

    void Window::DrawFrameBuffer()
    {
        // Bind the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Draw the framebuffer rectangle
        framebufferShader->use();
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    uint32_t Window::GetWidth() const
    {
        return windowData.Width;
    }

    uint32_t Window::GetHeight() const
    {
        return windowData.Height;
    }

    bool Window::IsVSync() const
    {
        return windowData.VSync;
    }

    void Window::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        
        windowData.VSync = enabled;
    }
}