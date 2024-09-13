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

			auto entity1 = activeScene->createEntity("Backpack");
			entity1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-1.0f, 0.0f, -5.0f)));
			entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));
			
			auto lightDir = activeScene->createEntity("Directionnal Light");
			lightDir->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.0f, 50.0f, 0.0f)));
			lightDir->AddComponent(std::make_shared<LightComponent>(LightType::DirectionnalLight));
			activeScene->AddSceneLight(lightDir);

			auto light1 = activeScene->createEntity("PointLight1");
			light1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(1.0f, 0.0f, 5.0f)));
			light1->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light1->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light1->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
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