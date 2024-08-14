#include <Scene.h>

namespace Moly
{
	unsigned int Scene::nextIndex = 0;

	Scene::Scene(std::string name) : name(name), index(nextIndex++), sceneEntities()
	{
		LogSceneCreation();
	}

	void Scene::LogSceneCreation()
	{
		ML_CORE_INFO("Created Scene {0} (id: {1})", name, index);
	}

	void Scene::AddEntity(Entity& _entity)
	{
		sceneEntities.push_back(_entity);
		ML_CORE_INFO("Entity {0} (id: {1}) added to Scene {2} (id: {3})", _entity.name, _entity.index,
			name, index);
	}

	void Scene::DrawEntities()
	{
		for (Entity entity : sceneEntities)
		{
			entity.Draw();
		}
	}
}