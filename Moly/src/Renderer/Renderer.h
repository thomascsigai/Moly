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
	
	private:
		Shader modelLoadingShader;
		Shader lightShader;
		Shader shadowMapShader;
		Shader framebufferShader;

		unsigned int FBO, RBO, framebufferTexture;
		unsigned int rectVAO, rectVBO;
		void InitFrameBuffer();
		void DrawFrameBuffer();

		void InitShadowMapFrameBuffer();

		void ResetLighting(Shader& shader);

		unsigned int shadowMapFBO;
		unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
		unsigned int shadowMap;

	};
}