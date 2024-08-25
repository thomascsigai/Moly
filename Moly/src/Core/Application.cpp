#include <Application.h>
#include <Vertices.h>
#include <Components.h>

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

		Scene scene = Scene();
		auto entity1 = scene.createEntity();
		entity1->addComponent(std::make_shared<TransformComponent>());
		entity1->addComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));

		while (!appWindow->Should_close())
		{
			appWindow->Clear();
			scene.Update();

			////scene1.DrawEntities();
			//// don't forget to enable shader before setting uniforms
			//Modelshader.use();

			//// view/projection transformations
			//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
			//glm::mat4 view = glm::mat4(1.0f);
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
			//Modelshader.setMat4("projection", projection);
			//Modelshader.setMat4("view", view);

			//// render the loaded model
			//glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			//Modelshader.setMat4("model", model);
			//backpack.Draw(Modelshader);


			DrawDebugWindow(scene);
			appWindow->Update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}

	void Application::DrawDebugWindow(Scene& scene) 
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(windowData.Title.c_str());

		/*if (ImGui::CollapsingHeader("Scene Camera", ImGuiTreeNodeFlags_DefaultOpen))
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
				ImGui::DragFloat("##MyDrag4", &currentScene.GetCurrentCam()->rotation.x, 0.1f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Y"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag5", &currentScene.GetCurrentCam()->rotation.y, 0.1f, -1000.0f, 1000.0f, "%.1f"); ImGui::SameLine();
				ImGui::Text("Z"); ImGui::SameLine();
				ImGui::SetNextItemWidth(50.0f);
				ImGui::DragFloat("##MyDrag6", &currentScene.GetCurrentCam()->rotation.z, 0.1f, -1000.0f, 1000.0f, "%.1f");
			}
			ImGui::NewLine();
		}*/

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