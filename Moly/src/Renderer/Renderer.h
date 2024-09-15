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
		Renderer();
		~Renderer() = default;


		void Render(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Entity> cam,
			const std::vector<std::shared_ptr<Entity>>& lights);

		bool msaa = true;
		float gamma = 2.2f;
		bool visualizeDepth = false;
		bool faceCulling = true;
		bool showWireframe = false;
		int selectedPostProcess = 0;
		glm::vec3 clearColor = glm::vec3(0.52f, 0.80f, 0.92f);

		float dofFocusDistance = 0.0f;
		float dofFocusRange = 0.0f;
		float dofMaxBlur = 0.0f;
	
	private:
		Shader modelLoadingShader;
		Shader lightShader;
		Shader shadowMapShader;
		Shader depthMapDebugShader;
		std::vector<Shader> postProcessShaders;

		unsigned int FBO, RBO, framebufferTexture;
		unsigned int rectVAO, rectVBO;
		void InitFrameBuffer();
		void DrawFrameBuffer();

		void ProcessRenderingSettings();

		unsigned int shadowMapFBO;
		unsigned int shadowMapWidth = 8192, shadowMapHeight = 8192;
		unsigned int shadowMap;
		glm::mat4 lightSpaceMatrix;
		void InitShadowMapFrameBuffer();
		void RenderSceneToShadowMap(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<Entity>>& lights);
		void DrawShadowMap();

	};
}