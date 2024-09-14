#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>

namespace Moly
{
	float rectangleVertices[] =
	{
		// Coords    // texCoords
		-1.0f, -1.0f,  0.0f, 0.0f,  // Bottom-left
		 1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
		-1.0f,  1.0f,  0.0f, 1.0f,  // Top-left

		 1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
		 1.0f,  1.0f,  1.0f, 1.0f,  // Top-right
		-1.0f,  1.0f,  0.0f, 1.0f   // Top-left
	};

	Renderer::Renderer() : 
		modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag"),
		lightShader("resources/shaders/light.vert", "resources/shaders/light.frag"),
		shadowMapShader("resources/shaders/shadow_map.vert", "resources/shaders/shadow_map.frag")
	{
		//Post-process shader initialisation
		postProcessShaders.push_back(Shader("resources/shaders/post-process/default.vert", "resources/shaders/post-process/default.frag"));
		postProcessShaders.push_back(Shader("resources/shaders/post-process/kernel.vert", "resources/shaders/post-process/kernel.frag"));
		postProcessShaders.push_back(Shader("resources/shaders/post-process/blur.vert", "resources/shaders/post-process/blur.frag"));
		postProcessShaders.push_back(Shader("resources/shaders/post-process/tv.vert", "resources/shaders/post-process/tv.frag"));
		postProcessShaders.push_back(Shader("resources/shaders/post-process/pixel.vert", "resources/shaders/post-process/pixel.frag"));
		postProcessShaders.push_back(Shader("resources/shaders/post-process/chrom_abb.vert", "resources/shaders/post-process/chrom_abb.frag"));

		FBO = 0;
		RBO = 0;
		framebufferTexture = 0;
		rectVBO = 0;
		rectVAO = 0;
		shadowMap = 0;
		shadowMapFBO = 0;

		InitFrameBuffer();
	}

	void Renderer::DrawFrameBuffer()
	{
	   // Bind the default framebuffer
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    // Draw the framebuffer rectangle
		postProcessShaders[selectedPostProcess].use();
		postProcessShaders[selectedPostProcess].setFloat("time", TimeManipulation::GameTime);
		postProcessShaders[selectedPostProcess].setFloat("pixelSize", 3.5f);
		postProcessShaders[selectedPostProcess].setFloat("blurAmount", 10.0f);
		postProcessShaders[selectedPostProcess].setFloat("chromAbbOffsetAmount", .05f);
		postProcessShaders[selectedPostProcess].setVec2("resolution", 1920, 1080);
	    glBindVertexArray(rectVAO);
	    glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	    glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer::Render(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Entity> cam,
		const std::vector<std::shared_ptr<Entity>>& lights)
	{
		ProcessRenderingSettings();

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
				currentShader->setBool("visualizeDepth", visualizeDepth);

				if (!lightComponent)
				{
					uint32_t pointLightCount = 0;
					uint32_t spotLightCount = 0;

					currentShader->setFloat("material.shininess", 400.0f);
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
				view = glm::rotate(view, glm::radians(-camTransform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				view = glm::rotate(view, glm::radians(-camTransform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				view = glm::rotate(view, glm::radians(-camTransform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				view = glm::translate(view, -camTransform->Position);
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
		if (selectedPostProcess != 0) DrawFrameBuffer();
	}

	void Renderer::ProcessRenderingSettings()
	{
		if (faceCulling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

		if (showWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (selectedPostProcess != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::InitFrameBuffer()
	{
		glUniform1i(glGetUniformLocation(postProcessShaders[selectedPostProcess].ID, "screenTexture"), 0);

		glGenVertexArrays(1, &rectVAO);
		glGenBuffers(1, &rectVBO);
		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		// Create Frame Buffer Object
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Create Framebuffer Texture
		glGenTextures(1, &framebufferTexture);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1600, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

		// Create Render Buffer Object
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1600, 900);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


		// Error checking framebuffer
		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			ML_CORE_ERROR("Framebuffer error: {0}\n", fboStatus);

	}
}