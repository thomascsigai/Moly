#pragma once

#include <Core.h>
#include <vector>
#include <Entity.h>
#include <Shader.h>

namespace Moly
{
	class MOLY_API Renderer
	{

    public:
		Renderer() = default;
		~Renderer() = default;


        void render(const std::vector<std::shared_ptr<Entity>>& entities)
        {
            Shader modelLoadingShader = Shader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag");

            for (const auto& entity : entities)
            {
                auto modelComponent = entity->getComponent<ModelComponent>();
                
                if (modelComponent)
                {
					modelLoadingShader.use();

					// view/projection transformations
					glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
					glm::mat4 view = glm::mat4(1.0f);
					view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
					modelLoadingShader.setMat4("projection", projection);
					modelLoadingShader.setMat4("view", view);

					// render the loaded model
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
					model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
					modelLoadingShader.setMat4("model", model);
                    modelComponent->model.Draw(modelLoadingShader);
                }
            }
        }
	};
}