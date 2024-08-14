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
		Entity* entity = new Entity("Test Triangle", vertices2, indices, glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 90.0f));
		//Entity* entity2 = new Entity("Test Triangle 2", vertices2, indices, glm::vec3(2.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		//Entity* entity3 = new Entity("Test Triangle 3", vertices2, indices, glm::vec3(2.0f), glm::vec3(1.0f), glm::vec3(0.0f));

		while (!appWindow->should_close())
		{
			appWindow->clear();

			shader.use();
			entity->Draw(shader);

			appWindow->update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}
}