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


		void render(const std::vector<std::shared_ptr<Entity>>& entities);

	private:
		Shader modelLoadingShader;
	};
}