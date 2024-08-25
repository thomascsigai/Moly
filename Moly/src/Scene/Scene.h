#pragma once

#include <Core.h>

#include <Entity.h>
#include <Log.h>
#include <Camera.h>

#include <vector>
#include <string>

namespace Moly
{
	class MOLY_API Scene
	{
	public:
		Scene(std::string _name);
		~Scene() = default;

		void AddEntity(Entity& _entity);
		void DrawEntities();

		Camera* GetCurrentCam();

	private:
		unsigned int index;
		static unsigned int nextIndex;

		std::string name;

		std::vector<Entity> sceneEntities;
		Camera currentCam;

		void LogSceneCreation();
	};
}