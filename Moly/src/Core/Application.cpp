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
		appWindow = new Window(windowData);
		activeScene = new Scene();

		if (activeScene == nullptr)
		{
			ML_CORE_ERROR("No active scene found, press a button to quit.");
			system("pause");
			return;
		}
		OnStart();

		ML_CORE_TRACE("Entering Main Render loop");
		while (!appWindow->Should_close())
		{
			appWindow->Clear();
			activeScene->Update();
			
			OnUpdate();
			
			#if DEBUG_MODE
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.8f));
				DrawDebugWindows();
			#endif

			appWindow->Update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}

	void Application::DrawDebugWindows() 
	{
		DrawSceneHierarchy();
		DrawPerformanceWindow();
		DrawRenderingWindow();

		ImGui::PopStyleColor();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Application::DrawSceneHierarchy()
	{
		static std::shared_ptr<Entity> selectedEntity = nullptr;

		ImGui::Begin("Scene Hierarchy");
		ImGui::Spacing();
		std::vector<std::shared_ptr<Entity>> entities = activeScene->GetEntities();

		for (int i = 0; i < entities.size(); i++)
		{
			ImGui::SetCursorPosX(20.0f);
			if (ImGui::Selectable(entities[i]->GetName().c_str(), selectedEntity == entities[i], ImGuiSelectableFlags_SpanAllColumns))
				selectedEntity = entities[i];
		}
		ImGui::Spacing();
		ImGui::End();

		DrawInspectorWindow(selectedEntity);
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

	void Application::DrawPerformanceWindow()
	{
		ImGui::Begin("Performance");

		ImGui::Spacing();
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
		ImGui::Spacing();

		ImGui::End();
	}

	void Application::DrawRenderingWindow()
	{
		ImGui::Begin("Rendering");
		ImGui::Spacing();

		auto renderer = activeScene->GetRenderer();

		ImGui::Checkbox("4x MSAA", &renderer->msaa);

		float color[3] = { renderer->clearColor[0], renderer->clearColor[1], renderer->clearColor[2] };
		ImGui::ColorEdit3("Clear Color", color);
		renderer->clearColor = glm::vec3(color[0], color[1], color[2]);

		ImGui::Checkbox("Face Culling", &renderer->faceCulling);

		ImGui::Checkbox("Visualize Depth", &renderer->visualizeDepth);

		ImGui::Checkbox("Show Wireframe", &renderer->showWireframe);

		ImGui::SliderFloat("Gamma", &renderer->gamma, 0.0f, 5.0f, "%.1f");

		ImGui::Combo("Post-Process", &renderer->selectedPostProcess, "None\0Depth Of Field (WIP)\0Kernel\0Blur\0TV Screen\0Pixelation\0Chromatic Aberration\0\0");
		
		if (renderer->selectedPostProcess == 1)
		{
			ImGui::SliderFloat("Focus Distance", &renderer->dofFocusDistance, 0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Focus Range", &renderer->dofFocusRange, 0.0f, 100.0f, "%.1f");
			ImGui::SliderFloat("Max Blur", &renderer->dofMaxBlur, 0.0f, 100.0f, "%.1f");
		}
		
		ImGui::Spacing();
		ImGui::End();
	}
}