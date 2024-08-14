#pragma once

#include <Core.h>
#include <Mesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moly
{
	class MOLY_API Entity
	{
	public:
		Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int>indices);
		Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int>indices, 
			glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
		
		~Entity() = default;

		void Draw(Shader&);

	private:
		unsigned int index;
		static unsigned int nextIndex;

		std::string name;

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		Mesh mesh;

		void LogEntityCreation();
		void ApplyTransformations(Shader&);
	};
}
