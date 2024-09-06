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
			entity1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(2.0f, -1.0f, -15.0f)));
			entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/cat/cat.obj"));
			
			auto entity2 = activeScene->createEntity("Backpack");
			entity2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-2.0f, 0.0f, -15.0f)));
			entity2->AddComponent(std::make_shared<ModelComponent>("resources/models/backpack/backpack.obj"));
			
			auto entity3 = activeScene->createEntity("Sponza");
			entity3->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.0f, -2.0f, -5.0f)));
			entity3->AddComponent(std::make_shared<ModelComponent>("resources/models/Sponza/sponza.obj"));

			auto lightDir = activeScene->createEntity("Directionnal Light");
			lightDir->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.0f, 10.0f, 0.0f)));
			lightDir->AddComponent(std::make_shared<LightComponent>(LightType::DirectionnalLight));
			activeScene->AddSceneLight(lightDir);

			auto light1 = activeScene->createEntity("PointLight1");
			light1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(25.8f, 4.8f, -16.67f)));
			light1->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light1->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light1->GetComponent<LightComponent>()->color = glm::vec3(0.98f, 0.61f, 0.2f);
			activeScene->AddSceneLight(light1);
			
			auto light2 = activeScene->createEntity("PointLight2");
			light2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(25.8f, 4.8f, 2.61f)));
			light2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light2->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light2->GetComponent<LightComponent>()->color = glm::vec3(0.98f, 0.61f, 0.2f);
			activeScene->AddSceneLight(light2);
			
			auto light3 = activeScene->createEntity("PointLight3");
			light3->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-32.95f, 4.8f, 2.61f)));
			light3->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light3->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light3->GetComponent<LightComponent>()->color = glm::vec3(0.98f, 0.61f, 0.2f);
			activeScene->AddSceneLight(light3);
			
			auto light4 = activeScene->createEntity("PointLight4");
			light4->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-32.95f, 4.8f, -16.63f)));
			light4->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light4->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light4->GetComponent<LightComponent>()->color = glm::vec3(0.98f, 0.61f, 0.2f);
			activeScene->AddSceneLight(light4);
			
			auto spotlight = activeScene->createEntity("Spotlight");
			spotlight->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-5.0f, 1.0f, -5.0f)));
			spotlight->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			spotlight->AddComponent(std::make_shared<LightComponent>(LightType::SpotLight));
			activeScene->AddSceneLight(spotlight);
		}

		void OnUpdate() override
		{

		}
	
	};

	Application* CreateApplication()
	{
		ML_CLIENT_TRACE("Initializing Sandbox App");
		Sandbox* sandbox = new Sandbox();
		sandbox->SetWindowData("Sandbox", 1920, 1080, true);

		return sandbox;
	}

}