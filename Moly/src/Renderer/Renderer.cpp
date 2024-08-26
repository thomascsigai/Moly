#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>

namespace Moly
{
	Renderer::Renderer() : 
		modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag")
	{}

	void Renderer::render(const std::vector<std::shared_ptr<Entity>>& entities, std::shared_ptr<Entity> cam)
	{
		std::shared_ptr<TransformComponent> camTransform = cam->GetComponent<TransformComponent>();
		std::shared_ptr<CameraComponent> camComponent = cam->GetComponent<CameraComponent>();

		for (const auto& entity : entities)
		{
			auto modelComponent = entity->GetComponent<ModelComponent>();

			if (modelComponent)
			{
				modelLoadingShader.use();

				// view/projection transformations
				glm::mat4 projection = camComponent->camera.projection;
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::rotate(view, camTransform->Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				view = glm::rotate(view, camTransform->Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
				view = glm::rotate(view, camTransform->Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				view = glm::translate(view, camTransform->Position);
				modelLoadingShader.setMat4("projection", projection);
				modelLoadingShader.setMat4("view", view);

				// render the loaded model
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, entity->GetComponent<TransformComponent>()->Position);
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, entity->GetComponent<TransformComponent>()->Scale);
				modelLoadingShader.setMat4("model", model);


				modelComponent->model.Draw(modelLoadingShader);
			}
		}
	}
}