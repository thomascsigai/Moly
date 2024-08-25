#pragma once

#include <Core.h>

#include <glm/glm.hpp>

namespace Moly
{
	class MOLY_API Camera
	{
	public:
		Camera() = default;
		~Camera() = default;
		
		float rotAroundValue = 0.0f;
		float rotUpValue = 1.0f;
		float distanceFromOrigin = 3.0f;

		bool lockView = true;
	};
}