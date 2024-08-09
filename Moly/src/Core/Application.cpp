#include <Application.h>

namespace Moly
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		appWindow = new Window(windowData);

		while (!appWindow->should_close())
		{
			appWindow->clear();
			appWindow->update();
		}
	}

	void Application::SetWindowData(WindowProps _windowData)
	{
		windowData = _windowData;
	}
}