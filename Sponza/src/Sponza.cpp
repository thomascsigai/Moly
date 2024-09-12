#include <Moly.h>

namespace Moly
{
	class Sponza : public Moly::Application
	{
	public:
		Sponza() : activeScene(nullptr)
		{

		}

		~Sponza()
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

			auto entity1 = activeScene->createEntity("Sponza");
			entity1->AddComponent(std::make_shared<TransformComponent>());
			entity1->GetComponent<TransformComponent>()->Scale = glm::vec3(.05f);
			entity1->AddComponent(std::make_shared<ModelComponent>("resources/models/Sponza/Sponza.gltf"));

			auto lightDir = activeScene->createEntity("Directionnal Light");
			lightDir->AddComponent(std::make_shared<TransformComponent>(glm::vec3(0.0f, 50.0f, 0.0f)));
			lightDir->AddComponent(std::make_shared<LightComponent>(LightType::DirectionnalLight));
			activeScene->AddSceneLight(lightDir);

			auto light1 = activeScene->createEntity("PointLight1");
			light1->AddComponent(std::make_shared<TransformComponent>(glm::vec3(25.8f, 4.8f, -16.67f)));
			light1->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light1->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light1->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light1);

			auto light2 = activeScene->createEntity("PointLight2");
			light2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(25.8f, 4.8f, 2.61f)));
			light2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light2->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light2->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light2);

			auto light3 = activeScene->createEntity("PointLight3");
			light3->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-32.95f, 4.8f, 2.61f)));
			light3->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light3->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light3->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light3);

			auto light4 = activeScene->createEntity("PointLight4");
			light4->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-32.95f, 4.8f, -16.63f)));
			light4->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light4->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light4->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light4);

			auto light5 = activeScene->createEntity("PointLight5");
			light5->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-57.87f, 26.30f, -30.32f)));
			light5->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light5->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light5->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light5);


			auto light6 = activeScene->createEntity("PointLight6");
			light6->AddComponent(std::make_shared<TransformComponent>(glm::vec3(57.87f, 26.30f, -30.32f)));
			light6->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light6->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light6->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light6);

			auto light7 = activeScene->createEntity("PointLight7");
			light7->AddComponent(std::make_shared<TransformComponent>(glm::vec3(57.87f, 26.30f, 16.24f)));
			light7->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light7->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light7->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light7);

			auto light8 = activeScene->createEntity("PointLight8");
			light8->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-57.87f, 26.30f, 16.24f)));
			light8->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			light8->AddComponent(std::make_shared<LightComponent>(LightType::PointLight));
			light8->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			activeScene->AddSceneLight(light8);

			auto spotlight = activeScene->createEntity("Spotlight");
			spotlight->AddComponent(std::make_shared<TransformComponent>(glm::vec3(-57.73f, 17.91f, -6.72f)));
			spotlight->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			spotlight->AddComponent(std::make_shared<LightComponent>(LightType::SpotLight));
			spotlight->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			spotlight->GetComponent<LightComponent>()->direction = glm::vec3(-1.92f, -1.59f, 0.0f);
			spotlight->GetComponent<LightComponent>()->cutOff = 33.76f;
			spotlight->GetComponent<LightComponent>()->outerCutOff = 40.0f;
			activeScene->AddSceneLight(spotlight);

			auto spotlight2 = activeScene->createEntity("Spotlight2");
			spotlight2->AddComponent(std::make_shared<TransformComponent>(glm::vec3(51.0f, 17.91f, -6.72f)));
			spotlight2->AddComponent(std::make_shared<ModelComponent>("resources/models/OBJ/box.obj"));
			spotlight2->AddComponent(std::make_shared<LightComponent>(LightType::SpotLight));
			spotlight2->GetComponent<LightComponent>()->SetColor(glm::vec3(0.98f, 0.61f, 0.2f));
			spotlight2->GetComponent<LightComponent>()->direction = glm::vec3(1.92f, -1.59f, 0.0f);
			spotlight2->GetComponent<LightComponent>()->cutOff = 33.76f;
			spotlight2->GetComponent<LightComponent>()->outerCutOff = 40.0f;
			activeScene->AddSceneLight(spotlight2);
		}

		void OnUpdate() override
		{

		}

	};

	Application* CreateApplication()
	{
		ML_CLIENT_TRACE("Initializing Sponza App");
		Sponza* sponza = new Sponza();
		sponza->SetWindowData("Sponza", 1600, 900, true);

		return sponza;
	}

}