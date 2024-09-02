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
			ImGui::Spacing();
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
			ImGui::Spacing();
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

		CameraComponent() : camera() 
		{}

		void DrawComponentInInspector() override
		{
			static int current_perspective = 0;
			ImGui::Spacing();
			ImGui::SetNextItemWidth(150.0f);
			ImGui::Combo("Projection Type", &current_perspective, "Perspective\0Orthographic\0\0");

			if (current_perspective == 0)
			{
				float fov = camera.perspectiveFOV;
				float nearPlane = camera.perspectiveNear;
				float farPlane = camera.perspectiveFar;

				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("FOV", &fov, 1.0f, 150.0f);
				ImGui::SeparatorText("Clipping Planes");
				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("Near", &nearPlane, 0.01f, 10000.0f, "%.1f");
				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("Far", &farPlane, 0.01f, 10000.0f, "%.1f");
				
				camera.SetPerspective(fov, nearPlane, farPlane);
			}
			else
			{
				float size = camera.orthoSize;
				float nearPlane = camera.orthoNear;
				float farPlane = camera.orthoFar;

				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("Size", &size, 1.0f, 150.0f);
				ImGui::SeparatorText("Clipping Planes");
				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("Near", &nearPlane, -1000.0f, 1000.0f, "%.1f");
				ImGui::SetNextItemWidth(150.0f);
				ImGui::SliderFloat("Far", &farPlane, -1000.0f, 1000.0f, "%.1f");

				camera.SetOrthographic(size, nearPlane, farPlane);
			}
			//ImGui::SliderFloat("FOV", &camera.perspectiveFOV, 1.0f, 150.0f);
		}
	};
}