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
			camera->AddComponent(std::make_shared<TransformComponent>());
			camera->AddComponent(std::make_shared<CameraComponent>());
			activeScene->SetPrimaryCam(camera);

			auto entity1 = activeScene->createEntity("Cat");
			entity1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(2.0f, -1.0f, -10.0f)));
			entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/cat/cat.obj"));
			
			auto entity2 = activeScene->createEntity("Backpack");
			entity2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-2.0f, 0.0f, -10.0f)));
			entity2->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));

			auto light1 = activeScene->createEntity("Light1");
			light1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(2.0f, 1.0f, -5.0f)));
			light1->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light1->AddComponent(std::make_shared<LightComponent>());
			activeScene->AddSceneLight(light1);
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