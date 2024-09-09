#pragma once

#include <Core.h>
#include <Window.h>
#include <Scene.h>

// This is the main class of the engine. It will be used by the client to create the application.
// The client will inherit from this class and override the virtual functions to implement the game logic.

namespace Moly
{
	class MOLY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync);
		
	protected:
		Scene* activeScene = nullptr;

	private:
		Window* appWindow = nullptr;
		WindowProps windowData = WindowProps();

		void DrawDebugWindows();
		void DrawSceneHierarchy();
		void DrawInspectorWindow(std::shared_ptr<Entity>);
		void DrawPerformanceWindow();
		void DrawRenderingWindow();

		virtual void OnUpdate() {}
		virtual void OnStart() {}
	};

	//To be defined in client
	Application* CreateApplication();
}

