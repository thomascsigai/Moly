#include <Entity.h>
#include <Log.h>
#include <TimeManipulation.h>

namespace Moly
{
	unsigned int Entity::nextIndex = 0;
	
	Entity::Entity(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shader _shader, bool _isLight) : 
		name(name), index(nextIndex++), position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), 
		mesh(vertices, indices), shader(_shader), isLight(_isLight)
	{
		LogEntityCreation();
	}

	Entity::Entity(std::string name, std::vector<Vertex>vertices, std::vector<unsigned int> indices, Shader _shader, bool _isLight,
		glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) :
		name(name), index(nextIndex++), position(position), scale(scale), rotation(rotation), isLight(_isLight),
		mesh(vertices, indices), shader(_shader)
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

	void Entity::Draw(Camera& camera)
	{
		shader.use();

		if (!isLight)
		{
			shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			shader.setVec3("lightPos", 3.0f, 2.0f, 5.0f);
			shader.setVec3("viewPos", camera.position.x, camera.position.y, camera.position.z);
		}

		ApplyTransformations(camera);

		mesh.Draw(shader);
	}

	void Entity::ApplyTransformations(Camera& camera)
	{
		glm::mat4 view = glm::mat4(1.0f);
		
		if (camera.lockView)
		{
			float camX = sin(camera.rotAroundValue) * camera.distanceFromOrigin;
			float camZ = cos(camera.rotAroundValue) * camera.distanceFromOrigin;
			float camY = camera.rotUpValue;

			camera.position = glm::vec3(camX, camY, camZ);

			view = glm::lookAt(camera.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else
		{
			view = glm::rotate(view, glm::radians(camera.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::rotate(view, glm::radians(camera.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(camera.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			view = glm::translate(view, camera.position);
		}

		//glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//// Rotate on himself
		//model = glm::rotate(model, glm::radians(50.0f) * TimeManipulation::GameTime, glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::translate(model, position);

		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}
}