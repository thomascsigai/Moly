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

		float gamma = 2.2f;
		bool visualizeDepth = false;
		bool faceCulling = true;
		bool showWireframe = false;
		int selectedPostProcess = 0;
	
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