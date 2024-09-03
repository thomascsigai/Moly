#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>

namespace Moly
{
	Renderer::Renderer() : 
		modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag"),
		lightShader("resources/shaders/light.vert", "resources/shaders/light.frag")
	{}

	void Renderer::render(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Entity> cam,
		const std::vector<std::shared_ptr<Entity>>& lights)
	{
		std::shared_ptr<TransformComponent> camTransform = cam->GetComponent<TransformComponent>();
		std::shared_ptr<CameraComponent> camComponent = cam->GetComponent<CameraComponent>();

		std::shared_ptr<LightComponent> light1Component = lights.at(0)->GetComponent<LightComponent>();

		for (const auto& entity : entities)
		{
			auto modelComponent = entity->GetComponent<ModelComponent>();
			auto lightComponent = entity->GetComponent<LightComponent>();

			if (modelComponent)
			{
				Shader* currentShader;

				if (lightComponent) currentShader = &lightShader;
				else currentShader = &modelLoadingShader;

				currentShader->use();

				if (!lightComponent)
				{
					currentShader->setVec3("light.position", lights.at(0)->GetComponent<TransformComponent>()->Position);
					currentShader->setVec3("viewPos", camTransform->Position);

					// light properties
					currentShader->setVec3("light.ambient", light1Component->ambient);
					currentShader->setVec3("light.diffuse", light1Component->diffuse);
					currentShader->setVec3("light.specular", light1Component->specular);

					currentShader->setFloat("light.constant", 1.0f);
					currentShader->setFloat("light.linear", 0.09f);
					currentShader->setFloat("light.quadratic", 0.032f);

					// material properties
					currentShader->setFloat("material.shininess", 64.0f);
				}
				else
				{
					currentShader->setVec3("lightColor", lightComponent->color);
				}

				// view/projection transformations
				glm::mat4 projection = camComponent->camera.projection;
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::rotate(view, camTransform->Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				view = glm::rotate(view, camTransform->Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
				view = glm::rotate(view, camTransform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				view = glm::translate(view, camTransform->Position);
				currentShader->setMat4("projection", projection);
				currentShader->setMat4("view", view);

				// render the loaded model
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, entity->GetComponent<TransformComponent>()->Position);
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, entity->GetComponent<TransformComponent>()->Scale);
				currentShader->setMat4("model", model);


				modelComponent->model.Draw(*currentShader);
				
				currentShader = NULL;
				delete currentShader;
			}
		}
	}
}