#pragma once

#include <Core.h>
#include <Entity.h>

namespace Moly
{
	class MOLY_API Light : public Entity
	{
	public:
		Light(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int>indices, Shader _shader,
			glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 lightColor) : 
			Entity(name, vertices, indices, _shader, position, scale, rotation), lightColor(lightColor) {};
		~Light() = default;

		glm::vec3 lightColor;

	private:
		void SetUniforms() override
		{
			shader.use();
		}
	};
}