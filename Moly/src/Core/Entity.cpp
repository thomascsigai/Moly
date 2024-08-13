#include <Entity.h>
#include <Log.h>

namespace Moly
{
	unsigned int Entity::nextIndex = 0;

	Entity::Entity(std::string name, std::vector<Vertex> vertices) : name(name), index(nextIndex++),
		position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), mesh(vertices, { 0 })
	{
		LogEntityCreation();
	}
	
	Entity::Entity(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices) : name(name), index(nextIndex++),
		position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), mesh(vertices, indices)
	{
		LogEntityCreation();
	}

	Entity::Entity(std::string name, std::vector<Vertex>vertices, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) :
		name(name), index(nextIndex++), position(position), scale(scale), rotation(rotation), mesh(vertices, { 0 })
	{
		LogEntityCreation();
	}

	void Entity::LogEntityCreation()
	{
		ML_CORE_INFO(
			"Created {0} entity with pos(x={1}, y={2}, z={3}), scale(x={4}, y={5}, z={6}), rotation(x={7}, y={8}, z={9})",
			name,
			position.x, position.y, position.z,
			scale.x, scale.y, scale.z,
			rotation.x, rotation.y, rotation.z
		);
	}

	void Entity::Draw(Shader& shader)
	{
		mesh.Draw(shader);
	}
}