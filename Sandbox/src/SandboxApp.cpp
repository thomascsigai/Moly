#include <Moly.h>

namespace Moly
{
	class Sandbox : public Moly::Application
	{
	public:
		Sandbox() : activeScene(nullptr)
		{
			
		}

		~Sandbox()
		{

		}

		Scene* activeScene;

		void OnStart() override
		{
			this->activeScene = Application::activeScene;

			auto camera = activeScene->createEntity("Main Camera");
			camera->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.0f, 0.0f, -10.0f)));
			camera->AddComponent(std::make_shared<CameraComponent>());
			activeScene->SetPrimaryCam(camera);

			auto entity1 = activeScene->createEntity("BackPack");
			entity1->AddComponent(std::make_shared<TransformComponent>());
			entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));

			auto entity2 = activeScene->createEntity("Cube");
			entity2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(1.0f)));
			entity2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
		}

		void OnUpdate() override
		{

		}
	
	};

	Application* CreateApplication()
	{
		ML_CLIENT_TRACE("Initializing Sandbox App");
		Sandbox* sandbox = new Sandbox();
		sandbox->SetWindowData("Sandbox", 1600, 900, true);

		return sandbox;
	}

}