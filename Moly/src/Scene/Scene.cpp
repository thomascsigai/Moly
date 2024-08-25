#include <Scene.h>

namespace Moly
{
    Scene::Scene(std::string _name)
        : name(_name), entities(), renderer() 
    {
        ML_CORE_INFO("Scene \"{0}\" created", name);
    }

    std::shared_ptr<Entity> Scene::createEntity()
    {
        auto entity = std::make_shared<Entity>(nextID++);
        entities.push_back(entity);
        ML_CORE_INFO("Entity {0} created and added to \"{1}\"", nextID, name);
        return entity;
    }

    void Scene::Update()
    {
        renderer.render(entities);
    }
}