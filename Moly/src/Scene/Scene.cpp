#include <Scene.h>

namespace Moly
{
    Scene::Scene(std::string _name)
        : name(_name), entities(), renderer() 
    {
        ML_CORE_INFO("Scene \"{0}\" created", name);
    }

    std::shared_ptr<Entity> Scene::createEntity(std::string _name)
    {
        auto entity = std::make_shared<Entity>(_name, nextID++);
        entities.push_back(entity);
        ML_CORE_INFO("Entity \"{0}\" (id: {1}) created and added to \"{2}\"", entity->GetName(), entity->GetID(), name);
        return entity;
    }

    void Scene::Update()
    {
        renderer.render(entities);
    }

    std::vector<std::shared_ptr<Entity>> Scene::GetEntities() const
    {
        return entities;
    }
}