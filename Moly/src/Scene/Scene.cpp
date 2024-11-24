#include <Scene.h>
#include <TimeManipulation.h>

namespace Moly
{
    Scene::Scene(std::string _name)
        : name(_name), entities(), renderer() 
    {
        ML_CORE_INFO("Scene \"{0}\" created", name);
    }

    void Scene::Update()
    {
        ProcessCameraMovements();
        if (entities.size() != 0) renderer.Render(entities, primaryCam, lights);
    }

    void Scene::ProcessCameraMovements()
    {
		auto camTransform = primaryCam->GetComponent<TransformComponent>();
        
    }

    std::shared_ptr<Entity> Scene::createEntity(std::string _name)
    {
        auto entity = std::make_shared<Entity>(_name, nextID++);
        entities.push_back(entity);
        ML_CORE_INFO("Entity \"{0}\" (id: {1}) created and added to \"{2}\"", entity->GetName(), entity->GetID(), name);
        return entity;
    }

    void Scene::SetPrimaryCam(std::shared_ptr<Entity> _cam)
    {
        auto component = _cam->GetComponent<CameraComponent>();
        if (component)
        {
            primaryCam = _cam;
            ML_CORE_INFO("Camera \"{0}\" set as primary cam for \"{1}\"", _cam->GetName(), name);
        }
    }

    void Scene::AddSceneLight(std::shared_ptr<Entity> _light)
    {
        if (!_light) return;
        lights.push_back(_light);
        ML_CORE_INFO("Light \"{0}\" added to \"{1}\"", _light->GetName(), name);
    }

    std::vector<std::shared_ptr<Entity>> Scene::GetEntities() const
    {
        return entities;
    }
}