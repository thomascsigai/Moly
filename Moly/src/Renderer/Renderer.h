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
	
	private:
		Shader modelLoadingShader;
		Shader lightShader;

		void ResetLighting(Shader& shader);

	};
}