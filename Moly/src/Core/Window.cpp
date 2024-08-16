#include <Window.h>
#include <TimeManipulation.h>

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
        glfwSetWindowUserPointer(windowGLFW, &windowData);
        glfwSetWindowPos(windowGLFW, 0, 0);

        glEnable(GL_DEPTH_TEST);

        InitImGui();
        ML_CORE_INFO("ImGui Initialized");

        ML_CORE_INFO("Window {0} created", props.Title, props.Width, props.Height);
	}

    void Window::InitImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;


        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(windowGLFW, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    bool Window::Should_close()
    {
        return glfwWindowShouldClose(windowGLFW);
    }

    void Window::Clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        {
            ML_CORE_INFO("VSync enabled");
            glfwSwapInterval(1);
        }
        else
        {
            ML_CORE_INFO("VSync disabled");
            glfwSwapInterval(0);
        }
        windowData.VSync = enabled;
    }
}