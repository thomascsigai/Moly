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

	void Camera::RecalculateProjection()
	{
		if (projectionType == ProjectionType::Perspective)
		{
			projection = glm::perspective(glm::radians(perspectiveFOV), aspectRatioW / aspectRatioH, perspectiveNear, perspectiveFar);
		}
	}
}