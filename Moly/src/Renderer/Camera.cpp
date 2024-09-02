#pragma once

#include <Camera.h>
#include <Log.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Moly
{
	Camera::Camera()
	{
		RecalculateProjection();
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		projectionType = ProjectionType::Perspective;
		perspectiveFOV = verticalFOV;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		RecalculateProjection();
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		projectionType = ProjectionType::Orthographic;
		orthoSize = size;
		orthoNear = nearClip;
		orthoFar = farClip;
		RecalculateProjection();
	}

	void Camera::RecalculateProjection()
	{
		float aspectRatio = aspectRatioW / aspectRatioH;

		if (projectionType == ProjectionType::Perspective)
		{
			projection = glm::perspective(glm::radians(perspectiveFOV), aspectRatio, perspectiveNear, perspectiveFar);
		}
		else
		{
			float orthoLeft = -orthoSize * aspectRatio * 0.5f;
			float orthoRight = orthoSize * aspectRatio * 0.5f;
			float orthoBottom = -orthoSize * 0.5f;
			float orthoTop = orthoSize * 0.5f;

			projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
		}
	}
}