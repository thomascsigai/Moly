#pragma once

#include <glm/glm.hpp>

#include <imgui.h>

#include <Camera.h>
#include <Model.h>

namespace Moly
{
	struct MOLY_API Component
	{
		virtual std::string GetName() { return "Component Name"; };

		virtual void DrawComponentInInspector()
		{
			ImGui::Text("No Component Data");
		};
	};

	struct MOLY_API TransformComponent : public Component
	{
		std::string GetName() override { return "Transform"; }
		
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}


		void DrawComponentInInspector() override
		{
			ImGui::Text("Position"); 
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("							 ").x);
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag", &Position.x, 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag2", &Position.y, 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag3", &Position.z, 0.01f, -1000.0f, 1000.0f, "%.2f");

			ImGui::Text("Rotation");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("							 ").x);
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag4", &Rotation.x, 0.1f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag5", &Rotation.y, 0.1f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag6", &Rotation.z, 0.1f, -1000.0f, 1000.0f, "%.2f");

			ImGui::Text("Scale");
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("							 ").x);
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag7", &Scale.x, 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag8", &Scale.y, 0.01f, -1000.0f, 1000.0f, "%.2f"); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::SetNextItemWidth(50.0f);
			ImGui::DragFloat("##MyDrag9", &Scale.z, 0.01f, -1000.0f, 1000.0f, "%.2f");
		}
	};

	struct MOLY_API ModelComponent : public Component
	{
		std::string GetName() override { return "Model"; }

		Model model;

		ModelComponent(std::string modelPath) 
			: model(modelPath, false) {}

		void DrawComponentInInspector() override
		{
			std::string label = "Model loaded : " + model.directory;
			ImGui::Text(label.c_str());
		}
	};

	struct MOLY_API CameraComponent : public Component
	{
		std::string GetName() override { return "Camera"; }

		Camera camera;

		CameraComponent() = default;

		void DrawComponentInInspector() override
		{
			ImGui::SliderFloat("FOV", &camera.perspectiveFOV, 1.0f, 150.0f);
		}
	};
}