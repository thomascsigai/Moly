#include <Application.h>
#include <Mesh.h>
#include <Entity.h>
#include <Vertices.h>

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
		ML_CORE_TRACE("Entering Main Render loop");
		appWindow = new Window(windowData);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		Shader shader = Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");

		std::vector<Vertex> vertices2;
		MOLY_DEFINE_TRIANGLE_VERTICES(vertices2);
		Moly::Entity entity = Moly::Entity("Test Triangle", vertices2, indices);

		while (!appWindow->should_close())
		{
			appWindow->clear();

			shader.use();
			entity.Draw(shader);

			appWindow->update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}
}