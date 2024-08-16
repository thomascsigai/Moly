#include <Application.h>
#include <Mesh.h>
#include <Entity.h>
#include <Vertices.h>
#include <Scene.h>

namespace Moly
{
	// Nombre maximum d'échantillons que nous allons stocker (par exemple 100)
	static const int MAX_FPS_SAMPLES = 100;

	// Tableau circulaire pour stocker les FPS
	static float fpsSamples[MAX_FPS_SAMPLES] = { 0 };
	static int currentSampleIndex = 0;

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		ML_CORE_TRACE("Entering Main Render loop");
		appWindow = new Window(windowData);

		Scene scene1 = Scene("Scene1");

		std::vector<Vertex> vertices;
		MOLY_DEFINE_CUBE_VERTICES(vertices);

		std::vector<unsigned int> indices{
			//Top
			2, 6, 7,
			2, 3, 7,

			//Bottom
			0, 4, 5,
			0, 1, 5,

			//Left
			0, 2, 6,
			0, 4, 6,

			//Right
			1, 3, 7,
			1, 5, 7,

			//Front
			0, 2, 3,
			0, 1, 3,

			//Back
			4, 6, 7,
			4, 5, 7
		};

		Shader shader = Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");

		Entity cube = Entity("Cube1", vertices, indices, shader);
		scene1.AddEntity(cube);

		while (!appWindow->Should_close())
		{
			appWindow->Clear();
			DrawDebugWindow();

			shader.use();
			scene1.DrawEntities();

			appWindow->Update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}

	void Application::DrawDebugWindow() 
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Exemple d'une fenêtre ImGui
		ImGui::Begin(windowData.Title.c_str());

		fpsSamples[currentSampleIndex] = ImGui::GetIO().Framerate;
		currentSampleIndex = (currentSampleIndex + 1) % MAX_FPS_SAMPLES;
		ImGui::PlotHistogram("FPS", fpsSamples, IM_ARRAYSIZE(fpsSamples), 0, nullptr, 0.0f, 120.0f, ImVec2(0, 80));
		ImGui::Text("Current FPS: %.1f", ImGui::GetIO().Framerate);

		ImGui::End();

		// Rendu d'ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}