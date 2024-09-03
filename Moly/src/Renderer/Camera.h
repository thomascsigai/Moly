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

		ProjectionType projectionType = ProjectionType::Perspective;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void RecalculateProjection();

		float perspectiveFOV = 70.0f;
		float perspectiveNear = 0.1f, perspectiveFar = 1000.0f;
		
		float orthoSize = 10.0f;
		float orthoNear = -1.0f, orthoFar = 1000.0f;
		
		float aspectRatioW = 1920.0f;
		float aspectRatioH = 1080.0f;

	};
}