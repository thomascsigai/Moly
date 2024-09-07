#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>

namespace Moly
{
	Renderer::Renderer() : 
		modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag"),
		lightShader("resources/shaders/light.vert", "resources/shaders/light.frag")
	{}

	void Renderer::ResetLighting(Shader& shader)
	{
		glm::vec3 zero = glm::vec3(0.0f);
		shader.setVec3("dirLight.direction", zero);
		shader.setVec3("dirLight.ambient", zero);
		shader.setVec3("dirLight.diffuse", zero);
		shader.setVec3("dirLight.specular", zero);
	}

	void Renderer::Render(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Entity> cam,
		const std::vector<std::shared_ptr<Entity>>& lights)
	{
		std::shared_ptr<TransformComponent> camTransform = cam->GetComponent<TransformComponent>();
		std::shared_ptr<CameraComponent> camComponent = cam->GetComponent<CameraComponent>();

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
					uint32_t pointLightCount = 0;
					uint32_t spotLightCount = 0;

					currentShader->setFloat("material.shininess", 100.0f);
					currentShader->setFloat("gamma", gamma);
					currentShader->setVec3("viewPos", camTransform->Position);

					for (int i = 0; i < lights.size(); i++)
					{
						auto light = lights[i]->GetComponent<LightComponent>();
						auto lightTransform = lights[i]->GetComponent<TransformComponent>();

						if (light->type == LightType::DirectionnalLight)
						{
							currentShader->setVec3("dirLight.direction", light->direction);
							currentShader->setVec3("dirLight.ambient", light->ambient);
							currentShader->setVec3("dirLight.diffuse", light->diffuse);
							currentShader->setVec3("dirLight.specular", light->specular);
						}
						else if (light->type == LightType::PointLight)
						{
							currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) +"].position", lightTransform->Position);
							currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) +"].ambient", light->ambient);
							currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) +"].diffuse", light->diffuse);
							currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) +"].specular", light->specular);
							currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) +"].constant", 1.0f);
							currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) +"].linear", 0.07f);
							currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) +"].quadratic", 0.017f);

							pointLightCount++;
						}
						else if (light->type == LightType::SpotLight)
						{
							currentShader->setVec3("spotLights[" + std::to_string(spotLightCount) + "].position", lightTransform->Position);
							currentShader->setVec3("spotLights[" + std::to_string(spotLightCount) + "].direction", light->direction);
							currentShader->setVec3("spotLights[" + std::to_string(spotLightCount) + "].ambient", light->ambient);
							currentShader->setVec3("spotLights[" + std::to_string(spotLightCount) + "].diffuse", light->diffuse);
							currentShader->setVec3("spotLights[" + std::to_string(spotLightCount) + "].specular", light->specular);
							currentShader->setFloat("spotLights[" + std::to_string(spotLightCount) + "].constant", 1.0f);
							currentShader->setFloat("spotLights[" + std::to_string(spotLightCount) + "].linear", 0.07f);
							currentShader->setFloat("spotLights[" + std::to_string(spotLightCount) + "].quadratic", 0.017f);
							currentShader->setFloat("spotLights[" + std::to_string(spotLightCount) + "].cutOff", glm::cos(glm::radians(light->cutOff)));
							currentShader->setFloat("spotLights[" + std::to_string(spotLightCount) + "].outerCutOff", glm::cos(glm::radians(light->outerCutOff)));

							spotLightCount++;
						}
					}

					currentShader->setInt("NB_POINT_LIGHTS", pointLightCount);
					currentShader->setInt("NB_SPOT_LIGHTS", spotLightCount);
				}
				else
				{
					currentShader->setVec3("lightColor", lightComponent->color);
				}

				// view/projection transformations
				glm::mat4 projection = camComponent->camera.projection;
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::rotate(view, glm::radians(camTransform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				view = glm::rotate(view, glm::radians(camTransform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				view = glm::rotate(view, glm::radians(camTransform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
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