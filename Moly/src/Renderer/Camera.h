#pragma once

#include <Core.h>

#include <glm/glm.hpp>

namespace Moly
{
	class MOLY_API Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		Camera() = default;
		~Camera() = default;
		
		bool lockViewToOrigin = false;

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
	};
}