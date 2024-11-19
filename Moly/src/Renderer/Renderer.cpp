#pragma once

#include <Renderer.h>
#include <TimeManipulation.h>
#include <glm/gtc/type_ptr.hpp>

namespace Moly
{
    // Rectangle pour le quad plein écran
    float rectangleVertices[] =
    {
        // Positions   // Coordonnées de texture
        -1.0f, -1.0f,  0.0f, 0.0f,  // Bas gauche
         1.0f, -1.0f,  1.0f, 0.0f,  // Bas droit
        -1.0f,  1.0f,  0.0f, 1.0f,  // Haut gauche

         1.0f, -1.0f,  1.0f, 0.0f,  // Bas droit
         1.0f,  1.0f,  1.0f, 1.0f,  // Haut droit
        -1.0f,  1.0f,  0.0f, 1.0f   // Haut gauche
    };

    Renderer::Renderer() :
        modelLoadingShader("resources/shaders/model_loading.vert", "resources/shaders/model_loading.frag"),
        lightShader("resources/shaders/light.vert", "resources/shaders/light.frag"),
        shadowMapShader("resources/shaders/shadow_map.vert", "resources/shaders/shadow_map.frag"),
		depthMapDebugShader("resources/shaders/shadow_map_visualization.vert", "resources/shaders/shadow_map_visualization.frag")
    {
        // Initialisation des shaders de post-traitement
        postProcessShaders.push_back(Shader("resources/shaders/post-process/default.vert", "resources/shaders/post-process/default.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/dof.vert", "resources/shaders/post-process/dof.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/kernel.vert", "resources/shaders/post-process/kernel.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/blur.vert", "resources/shaders/post-process/blur.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/tv.vert", "resources/shaders/post-process/tv.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/pixel.vert", "resources/shaders/post-process/pixel.frag"));
        postProcessShaders.push_back(Shader("resources/shaders/post-process/chrom_abb.vert", "resources/shaders/post-process/chrom_abb.frag"));

        // Initialisation des variables
        FBO = 0;
        RBO = 0;
        framebufferTexture = 0;
        rectVBO = 0;
        rectVAO = 0;
        shadowMap = 0;
        shadowMapFBO = 0;
		lightSpaceMatrix = glm::mat4(1.0f);

        InitFrameBuffer();
        InitShadowMapFrameBuffer();
    }

    void Renderer::DrawFrameBuffer()
    {
        // Lier le framebuffer par défaut
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Désactiver le test de profondeur pour éviter que le quad ne soit rejeté
        glDisable(GL_DEPTH_TEST);

        // Utiliser le shader de post-traitement sélectionné
        postProcessShaders[selectedPostProcess].use();
        postProcessShaders[selectedPostProcess].setFloat("time", TimeManipulation::GameTime);
        postProcessShaders[selectedPostProcess].setFloat("pixelSize", 3.5f);
        postProcessShaders[selectedPostProcess].setFloat("blurAmount", 10.0f);
        postProcessShaders[selectedPostProcess].setFloat("chromAbbOffsetAmount", .05f);
        postProcessShaders[selectedPostProcess].setVec2("resolution", 1920, 1080);
        
        postProcessShaders[selectedPostProcess].setFloat("focusDistance", dofFocusDistance);
        postProcessShaders[selectedPostProcess].setFloat("focusRange", dofFocusRange);
        postProcessShaders[selectedPostProcess].setFloat("maxBlur", dofMaxBlur);

        glBindVertexArray(rectVAO);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Réactiver le test de profondeur
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::DrawShadowMap()
    {
        // Optionnel : Afficher la shadow map dans un coin de l'écran
        glViewport(0, 0, 1600 / 4, 900 / 4);

        // Lier le framebuffer par défaut et désactiver le test de profondeur
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        // Utiliser le shader shadowMapShader pour la visualisation
        depthMapDebugShader.use();
        depthMapDebugShader.setInt("depthMap", 0);

        // Activer la texture de la shadow map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);

        glBindVertexArray(rectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Réactiver le test de profondeur
        glEnable(GL_DEPTH_TEST);

        // Réinitialiser le viewport si nécessaire
        // glViewport(0, 0, windowWidth, windowHeight);
    }

    void Renderer::Render(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Entity> cam,
        const std::vector<std::shared_ptr<Entity>>& lights)
    {
        std::shared_ptr<TransformComponent> camTransform = cam->GetComponent<TransformComponent>();
        std::shared_ptr<CameraComponent> camComponent = cam->GetComponent<CameraComponent>();
        
        // Rendu de la scène depuis la perspective de la lumière pour générer la shadow map
        RenderSceneToShadowMap(entities, lights);
        
        ProcessRenderingSettings();

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

                    currentShader->setFloat("material.shininess", modelComponent->shininess);
                    currentShader->setFloat("gamma", gamma);
                    currentShader->setVec3("viewPos", camTransform->Position);

                    // Configuration des lumières
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
                            currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) + "].position", lightTransform->Position);
                            currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) + "].ambient", light->ambient);
                            currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) + "].diffuse", light->diffuse);
                            currentShader->setVec3("pointLights[" + std::to_string(pointLightCount) + "].specular", light->specular);
                            currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) + "].constant", 1.0f);
                            currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) + "].linear", 0.07f);
                            currentShader->setFloat("pointLights[" + std::to_string(pointLightCount) + "].quadratic", 0.017f);

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

                    // Configurer la shadow map
                    currentShader->setInt("shadowMap", 1);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, shadowMap);

                    // Matrice de transformation de la lumière
                    currentShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
                }
                else
                {
                    currentShader->setVec3("lightColor", lightComponent->color);
                }

                // Transformations de vue et de projection
                glm::mat4 projection = camComponent->camera.projection;
                glm::mat4 view = glm::mat4(1.0f);
                view = glm::rotate(view, glm::radians(-camTransform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                view = glm::rotate(view, glm::radians(-camTransform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                view = glm::rotate(view, glm::radians(-camTransform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                view = glm::translate(view, -camTransform->Position);
                currentShader->setMat4("projection", projection);
                currentShader->setMat4("view", view);

                // Transformation du modèle
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, entity->GetComponent<TransformComponent>()->Position);
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, entity->GetComponent<TransformComponent>()->Scale);
                currentShader->setMat4("model", model);

                modelComponent->model.Draw(*currentShader);
            }
        }

        if (selectedPostProcess != 0)
            DrawFrameBuffer();

        // Afficher la shadow map à l'écran
        //DrawShadowMap();
    }

    void Renderer::ProcessRenderingSettings()
    {
        if (faceCulling) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);

        if (showWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (msaa) glEnable(GL_MULTISAMPLE);
        else glDisable(GL_MULTISAMPLE);

        if (selectedPostProcess != 0)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
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

        // Création du Framebuffer Object
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Création de la texture du framebuffer
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Évite les fuites de texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Évite les fuites de texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Création du Render Buffer Object
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // Vérification du framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            ML_CORE_ERROR("Framebuffer error: {0}\n", fboStatus);
    }

    void Renderer::InitShadowMapFrameBuffer()
    {
        // Génération du framebuffer pour la shadow map
        glGenFramebuffers(1, &shadowMapFBO);

        // Génération de la texture pour la shadow map
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // Évite l'assombrissement en dehors du frustum
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Vérification du framebuffer de la shadow map
        auto shadowMapFBOStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (shadowMapFBOStatus != GL_FRAMEBUFFER_COMPLETE)
            ML_CORE_ERROR("Shadow Map Framebuffer error: {0}\n", shadowMapFBOStatus);

    }

    void Renderer::RenderSceneToShadowMap(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<Entity>>& lights)
    {
        // Configurer le framebuffer pour la shadow map
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

        glm::vec3 lightPos;

        // Configuration des lumières
        for (int i = 0; i < lights.size(); i++)
        {
            auto light = lights[i]->GetComponent<LightComponent>();
            auto lightTransform = lights[i]->GetComponent<TransformComponent>();

            if (light->type == LightType::DirectionnalLight)
            {
                lightPos = glm::vec3(lightTransform->Position.x, 250, lightTransform->Position.z);
                //lightPos = lightTransform->Position;
                light->direction = glm::normalize(-lightPos);
            }
        }

        // Position et direction de la lumière
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Matrice de projection pour la lumière directionnelle
        float near_plane = 1.0f, far_plane = 1000.0f;
        glm::mat4 orthoProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);

        lightSpaceMatrix = orthoProjection * lightView;

        shadowMapShader.use();
        shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Rendu des entités pour générer la shadow map
        for (const auto& entity : entities)
        {
            auto modelComponent = entity->GetComponent<ModelComponent>();
            if (modelComponent)
            {
                // Transformation du modèle
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, entity->GetComponent<TransformComponent>()->Position);
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(entity->GetComponent<TransformComponent>()->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, entity->GetComponent<TransformComponent>()->Scale);
                shadowMapShader.setMat4("model", model);

                modelComponent->model.Draw(shadowMapShader);
            }
        }

        // Réinitialiser le framebuffer par défaut
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080); // Remplacez par la taille de votre fenêtre
		if (faceCulling) glEnable(GL_CULL_FACE);
    }
}
