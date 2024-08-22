#pragma once

#include <Core.h>
#include <Mesh.h>
#include <Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moly
{
	class MOLY_API Entity
	{
	public:
		Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int>indices, Shader _shader, bool _isLight);
		Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int>indices, Shader _shader, bool _isLight,
			glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
		
		~Entity() = default;

		void Draw(Camera& camera);

		unsigned int index;
		std::string name;


	private:
		static unsigned int nextIndex;

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		Mesh mesh;

		Shader shader;

		void LogEntityCreation();
		void ApplyTransformations(Camera& camera);

		bool isLight;
	};
}
