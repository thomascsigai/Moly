#include <Application.h>
#include <Mesh.h>
#include <Entity.h>
#include <Vertices.h>
#include <Scene.h>

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

		Scene scene1 = Scene("Scene1");

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		Shader shader = Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");

		std::vector<Vertex> vertices2;
		MOLY_DEFINE_TRIANGLE_VERTICES(vertices2);
		Entity entity = Entity("Test Triangle", vertices2, indices, shader, glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 90.0f));
		Entity entity2 = Entity("Test Triangle 2", vertices2, indices, shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
		//Entity* entity3 = new Entity("Test Triangle 3", vertices2, indices, glm::vec3(2.0f), glm::vec3(1.0f), glm::vec3(0.0f));

		scene1.AddEntity(entity);
		scene1.AddEntity(entity2);

		while (!appWindow->should_close())
		{
			appWindow->clear();

			shader.use();
			scene1.DrawEntities();

			appWindow->update();
		}
	}

	void Application::SetWindowData(std::string Title, uint32_t width, uint32_t height, bool VSync)
	{
		WindowProps _windowData = { Title, width, height, VSync };
		windowData = _windowData;
	}
}