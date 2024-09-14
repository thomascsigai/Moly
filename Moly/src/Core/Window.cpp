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

	static void GLFWErrorCallback(int error, const char* description)
	{
		ML_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Window::Window(const WindowProps& props) : windowData(props)
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
		glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        InitImGui();
        ML_CORE_INFO("ImGui Initialized");

        ML_CORE_INFO("Window {0} created", props.Title, props.Width, props.Height);
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