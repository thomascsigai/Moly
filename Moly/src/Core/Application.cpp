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
		Shader lightShader = Shader("resources/shaders/basic.vert", "resources/shaders/light.frag");
		
		Entity cube = Entity("Cube1", vertices, indices, shader, false);
		Entity light = Entity("Light1", vertices, indices, lightShader, true, glm::vec3(2.0f), glm::vec3(.5f), glm::vec3(0.0f));

		scene1.AddEntity(cube);
		scene1.AddEntity(light);

		while (!appWindow->Should_close())
		{
			appWindow->Clear();

			scene1.DrawEntities();

			DrawDebugWindow(scene1);
			appWindow->Update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}

	void Application::DrawDebugWindow(Scene& currentScene) 
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(windowData.Title.c_str());

		if (ImGui::CollapsingHeader("Scene Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Lock View on Scene Origin", &currentScene.GetCurrentCam()->lockView);

			if (currentScene.GetCurrentCam()->lockView)
			{
				ImGui::SetNextItemWidth(80.0f);
				ImGui::DragFloat("Rot Around", &currentScene.GetCurrentCam()->rotAroundValue, 0.01f, -1000.0f, 1000.0f, "%.1f");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::DragFloat("Elevation", &currentScene.GetCurrentCam()->rotUpValue, 0.01f, -1000.0f, 1000.0f, "%.1f");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::DragFloat("Distance From Origin", &currentScene.GetCurrentCam()->distanceFromOrigin, 0.01f, 0.0f, 1000.0f, "%.1f");
			}
			else
			{
				ImGui::Text("Position		"); ImGui::SameLine();
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f); 
				ImGui::DragFloat("##MyDrag", &currentScene.GetCurrentCam()->position.x, 0.01f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag2", &currentScene.GetCurrentCam()->position.y, 0.01f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Z"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag3", &currentScene.GetCurrentCam()->position.z, 0.01f, -1000.0f, 1000.0f, "%.1f");
				
				ImGui::Text("Rotation		"); ImGui::SameLine();
				ImGui::Text("X"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f); 
				ImGui::DragFloat("##MyDrag4", &currentScene.GetCurrentCam()->rotation.x, 0.01f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag5", &currentScene.GetCurrentCam()->rotation.y, 0.01f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Z"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag6", &currentScene.GetCurrentCam()->rotation.z, 0.01f, -1000.0f, 1000.0f, "%.1f");
			}
			ImGui::NewLine();
		}

		if (ImGui::CollapsingHeader("Performance"))
		{

			fpsSamples[currentSampleIndex] = ImGui::GetIO().Framerate;
			currentSampleIndex = (currentSampleIndex + 1) % MAX_FPS_SAMPLES;

			float average = 0.0f;
			for (int n = 0; n < IM_ARRAYSIZE(fpsSamples); n++)
				average += fpsSamples[n];
			average /= (float)IM_ARRAYSIZE(fpsSamples);
			char overlay[32];
			sprintf(overlay, "avg %f fps", average);
			ImGui::PlotHistogram("Framerate", fpsSamples, IM_ARRAYSIZE(fpsSamples), 0, overlay, 0.0f, 250.0f, ImVec2(0, 80.0f));
		
			bool VSync = appWindow->IsVSync();
			ImGui::Checkbox("Enable VSync", &VSync);
			appWindow->SetVSync(VSync);
			
			ImGui::NewLine();
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}