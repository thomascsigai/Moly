#include <Entity.h>
#include <Log.h>

namespace Moly
{
	unsigned int Entity::nextIndex = 0;
	
	Entity::Entity(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shader _shader) : name(name), index(nextIndex++),
		position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), mesh(vertices, indices), shader(_shader)
	{
		LogEntityCreation();
	}

	Entity::Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int> indices, Shader _shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) :
		name(name), index(nextIndex++), position(position), scale(scale), rotation(rotation), mesh(vertices, indices), shader(_shader)
	{
		LogEntityCreation();
	}

	void Entity::LogEntityCreation()
	{
		ML_CORE_INFO(
			"Created {0} entity (id = {10}) with pos(x={1}, y={2}, z={3}), scale(x={4}, y={5}, z={6}), rotation(x={7}, y={8}, z={9})",
			name,
			position.x, position.y, position.z,
			scale.x, scale.y, scale.z,
			rotation.x, rotation.y, rotation.z,
			index
		);
	}

	void Entity::Draw()
	{
		ApplyTransformations();
		mesh.Draw(shader);
	}

	void Entity::ApplyTransformations()
	{
		glm::mat4 view = glm::mat4(1.0f);
		//view = glm::lookAt(glm::vec3(camX, 1.5f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(60.0f), 1600.0f / 900.0f, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, position);

		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}
}