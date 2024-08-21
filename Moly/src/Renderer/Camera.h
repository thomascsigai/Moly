#pragma once

#include <Core.h>

#include <glm/glm.hpp>

namespace Moly
{
	class MOLY_API Camera
	{
	public:
		Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3 _rotation = glm::vec3(0.0f)) : 
			position(_position), rotation(_rotation) {};
		~Camera() = default;

		glm::vec3 position;
		glm::vec3 rotation;
		
		float rotAroundValue = 0.0f;
		float rotUpValue = 1.0f;
		float distanceFromOrigin = 3.0f;

		bool lockView = true;
	};
}