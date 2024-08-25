#pragma once

#include <glm/glm.hpp>

#include <Camera.h>
#include <Model.h>

namespace Moly
{
	struct MOLY_API Component
	{};

	struct MOLY_API TransformComponent : public Component
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}
	};

	struct MOLY_API ModelComponent : public Component
	{
		Model model;

		ModelComponent(std::string modelPath) 
			: model(modelPath, false) {}
	};

	struct MOLY_API CameraComponent : public Component
	{
		Camera camera;

		CameraComponent() = default;
	};
}