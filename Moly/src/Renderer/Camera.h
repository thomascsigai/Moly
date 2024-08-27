#pragma once

#include <Core.h>

#include <glm/glm.hpp>

namespace Moly
{
	class MOLY_API Camera
	{
	public:
		enum class ProjectionType { Perspective, Orthographic };
	public:
		Camera();
		~Camera() = default;

		glm::mat4 projection = glm::mat4(1.0f);
		
		bool lockViewToOrigin = false;

		ProjectionType projectionType = ProjectionType::Perspective;

		float perspectiveFOV = 45.0f;
		float perspectiveNear = 0.01f, perspectiveFar = 10000.0f;
		float aspectRatioW = 1920.0f;
		float aspectRatioH = 1080.0f;

	private:

		void RecalculateProjection();
	};
}