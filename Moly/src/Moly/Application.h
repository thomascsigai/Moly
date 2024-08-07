#pragma once

#include "Core.h"

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
	};

	//To be defined in client
	Application* CreateApplication();
}

