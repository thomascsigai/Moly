#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>

namespace Moly
{
	Renderer::Renderer() : 
		modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag")
	{}

	void Renderer::render(const std::vector<std::shared_ptr<Entity>>& entities)
	{
		for (const auto& entity : entities)
		{
			auto modelComponent = entity->GetComponent<ModelComponent>();

			if (modelComponent)
			{
				modelLoadingShader.use();

				// view/projection transformations
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
				modelLoadingShader.setMat4("projection", projection);
				modelLoadingShader.setMat4("view", view);

				// render the loaded model
				glm::mat4 model = glm::mat4(1.0f);
				//model = glm::rotate(model, glm::radians(90.0f) * TimeManipulation::GameTime, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, entity->GetComponent<TransformComponent>()->Translation); // translate it down so it's at the center of the scene
				model = glm::scale(model, entity->GetComponent<TransformComponent>()->Scale);	// it's a bit too big for our scene, so scale it down
				modelLoadingShader.setMat4("model", model);
				modelComponent->model.Draw(modelLoadingShader);
			}
		}
	}
}