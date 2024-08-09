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

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}
}