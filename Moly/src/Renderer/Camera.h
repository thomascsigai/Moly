#pragma once

#include <Core.h>

#include <glm/glm.hpp>

namespace Moly
{
	class MOLY_API Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};
	public:
		Camera();
		~Camera() = default;

		glm::mat4 projection = glm::mat4(1.0f);

		ProjectionType projectionType = ProjectionType::Orthographic;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void RecalculateProjection();

		float perspectiveFOV = 70.0f;
		float perspectiveNear = 0.1f, perspectiveFar = 1000.0f;
		
		float orthoSize = 30.0f;
		float orthoNear = -1.0f, orthoFar = 1000.0f;
		
		float aspectRatioW = 1920.0f;
		float aspectRatioH = 1080.0f;

	};
}