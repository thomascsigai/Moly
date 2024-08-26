#include <Application.h>
#include <Vertices.h>
#include <Components.h>

namespace Moly
{
	static const int MAX_FPS_SAMPLES = 100;

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
		
		auto camera = scene.createEntity("Main Camera");
		camera->AddComponent(std::make_shared<TransformComponent>());
		camera->AddComponent(std::make_shared<CameraComponent>());

		auto entity1 = scene.createEntity("BackPack");
		entity1->AddComponent(std::make_shared<TransformComponent>());
		entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));

		auto entity2 = scene.createEntity("Box");
		entity2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(1.0f)));
		entity2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/Box.obj"));

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

		static std::shared_ptr<Entity> selectedEntity = nullptr;
		
		if (ImGui::CollapsingHeader("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<std::shared_ptr<Entity>> entities = scene.GetEntities();
			
			for (int i = 0; i < entities.size(); i++)
			{
				ImGui::SetCursorPosX(20.0f);
				if (ImGui::Selectable(entities[i]->GetName().c_str(), selectedEntity == entities[i], ImGuiSelectableFlags_SpanAllColumns))
					selectedEntity = entities[i];
			}
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

		DrawInspectorWindow(selectedEntity);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Application::DrawInspectorWindow(std::shared_ptr<Entity> selectedEntity)
	{
		ImGui::Begin("Inspector");

		if (selectedEntity)
		{
			std::string label = selectedEntity->GetName() + " (id: " + std::to_string(selectedEntity->GetID()) + ")";
			ImGui::Text(label.c_str());

			for (auto& [key, component] : selectedEntity->GetComponents())
			{
				if (ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					component->DrawComponentInInspector();
			}
		}

		ImGui::End();
	}
}