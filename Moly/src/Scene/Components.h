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

		glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

		void Translate(glm::vec3 translation)
		{
			Position += translation;
		}

		void updateCameraVectors()
		{
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
			front.y = sin(glm::radians(Rotation.x));
			front.z = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
			Front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
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
			ImGui::Spacing();
			std::string label = "Model loaded : " + model.directory;
			ImGui::Text(label.c_str());
			ImGui::Spacing();
		}
	};

	struct MOLY_API CameraComponent : public Component
	{
		std::string GetName() override { return "Camera"; }
		
		float freeCamSpeed = 5.0f;

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

				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("FOV", &fov, 1.0f, 150.0f);
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text("Clipping Planes");
				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("Near", &nearPlane, 0.1f, 10000.0f, "%.1f");
				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("Far", &farPlane, 0.1f, 10000.0f, "%.1f");
				
				camera.SetPerspective(fov, nearPlane, farPlane);
			}
			else
			{
				float size = camera.orthoSize;
				float nearPlane = camera.orthoNear;
				float farPlane = camera.orthoFar;

				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("Size", &size, 1.0f, 150.0f);
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text("Clipping Planes");
				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("Near", &nearPlane, -1000.0f, 1000.0f, "%.1f");
				ImGui::SetNextItemWidth(210.0f);
				ImGui::SliderFloat("Far", &farPlane, -1000.0f, 1000.0f, "%.1f");

				camera.SetOrthographic(size, nearPlane, farPlane);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderFloat("Free Cam Speed", &freeCamSpeed, 0.1f, 20.0f, "%.1f");

			ImGui::Spacing();
		}
	};

	enum LightType
	{
		PointLight = 0,
		DirectionnalLight = 1,
		SpotLight = 2
	};

	struct MOLY_API LightComponent : public Component
	{
		std::string GetName() override { return "Light"; }

		LightComponent() {}
		LightComponent(LightType _lightType) : type(_lightType) {}

		glm::vec3 color = glm::vec3(1.0f);

		LightType type = LightType::PointLight;

		glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);

		float cutOff = 12.5f;
		float outerCutOff = 15.0f;

		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specular = glm::vec3(0.1f, 0.1f, 0.1f);

		void DrawComponentInInspector() override
		{
			int light_type = type;

			ImGui::Spacing();
			ImGui::SetNextItemWidth(195.0f);
			ImGui::Combo("Type", &light_type, "Point Light\0Directionnal Light\0Spot Light\0\0");
			
			if (light_type == 0) type = LightType::PointLight;
			if (light_type == 1)
			{
				type = LightType::DirectionnalLight;
				float dir[3] = { direction.x, direction.y, direction.z };
				ImGui::DragFloat3("Direction", dir, 0.01f, -1000.0f, 1000.0f, "%.2f");
				direction = glm::vec3(dir[0], dir[1], dir[2]);
			}
			if (light_type == 2)
			{
				type = LightType::SpotLight;
				float dir[3] = { direction.x, direction.y, direction.z };
				ImGui::DragFloat3("Direction", dir, 0.01f, -1000.0f, 1000.0f, "%.2f");
				direction = glm::vec3(dir[0], dir[1], dir[2]);
				ImGui::SliderFloat("CutOff", &cutOff, 0.0f, 40.0f, "%.2f");
				ImGui::SliderFloat("OuterCutOff", &outerCutOff, 0.0f, 40.0f, "%.2f");
			}

			ImVec4 colorPicked = ImVec4(color.x, color.y, color.z, 1.0f);
			
			ImGui::ColorEdit3("Color", (float*)&colorPicked);
			color = glm::vec3(colorPicked.x, colorPicked.y, colorPicked.z);
			ImGui::Spacing();
			ImGui::Spacing();
			
			ImGui::SliderFloat("Ambient", &ambientFactor, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("Diffuse", &diffuseFactor, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("Specular", &specularFactor, 0.0f, 1.0f, "%.2f");

			ambient = color * ambientFactor;
			diffuse = color * diffuseFactor;
			specular = color * specularFactor;

			ImGui::Spacing();
		}

	private:
		float ambientFactor = 0.1f;
		float diffuseFactor = 0.5f;
		float specularFactor = 0.1f;
	};
}