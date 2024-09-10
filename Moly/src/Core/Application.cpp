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
			
			appWindow->DrawFrameBuffer();
			
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

		static bool faceCulling = true;
		ImGui::Checkbox("Face Culling", &faceCulling);

		if (faceCulling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

		ImGui::Checkbox("Visualize Depth", &activeScene->GetRenderer()->visualizeDepth);

		static bool showWireframe = false;
		ImGui::Checkbox("Show Wireframe", &showWireframe);

		if (showWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ImGui::SliderFloat("Gamma", &activeScene->GetRenderer()->gamma, 0.0f, 5.0f, "%.1f");

		ImGui::Combo("Post-Process", &appWindow->useFrameBuffer, "None\0Kernel\0\0");
		ImGui::Spacing();

		ImGui::End();
	}
}