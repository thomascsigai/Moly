#include <Application.h>
#include <Vertices.h>
#include <Components.h>

namespace Moly
{
	// Nombre maximum d'�chantillons que nous allons stocker (par exemple 100)
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
		auto entity1 = scene.createEntity("Spider");
		entity1->AddComponent(std::make_shared<TransformComponent>());
		entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));

		auto entity2 = scene.createEntity("Box");
		entity2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(1.0f)));
		entity2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));

		while (!appWindow->Should_close())
		{
			appWindow->Clear();
			scene.Update();

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

		if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<std::shared_ptr<Entity>> entities = scene.GetEntities();
				
			for (int i = 0; i < entities.size(); i++)
			{
				std::shared_ptr<TransformComponent> transform = entities[i]->GetComponent<TransformComponent>();
				ImGui::PushID(i);
				if (ImGui::TreeNode("", "%s", entities[i]->GetName().c_str()))
				{
					if (!transform)
					{
						ImGui::Text("No Transform");
						ImGui::TreePop();
						ImGui::PopID();
						continue;
					}

					ImGui::Text("Position		"); ImGui::SameLine();
					ImGui::Text("X"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag", &(transform->Translation.x), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Y"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag2", &(transform->Translation.y), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Z"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag3", &(transform->Translation.z), 0.01f, -1000.0f, 1000.0f, "%.2f");

					ImGui::Text("Rotation		"); ImGui::SameLine();
					ImGui::Text("X"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag4", &(transform->Rotation.x), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Y"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag5", &(transform->Rotation.y), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Z"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag6", &(transform->Rotation.z), 0.01f, -1000.0f, 1000.0f, "%.2f");

					ImGui::Text("Scale		"); ImGui::SameLine();
					ImGui::Text("X"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag7", &(transform->Scale.x), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Y"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag8", &(transform->Scale.y), 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
					ImGui::Text("Z"); ImGui::SameLine();
					ImGui::SetNextItemWidth(50.0f);
					ImGui::DragFloat("##MyDrag9", &(transform->Scale.z), 0.01f, -1000.0f, 1000.0f, "%.2f");
					ImGui::TreePop();

				}
				ImGui::PopID();
			}
		}

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