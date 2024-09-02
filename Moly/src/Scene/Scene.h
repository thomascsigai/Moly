#pragma once

#include <Core.h>

#include <Entity.h>
#include <Log.h>
#include <Renderer.h>

#include <vector>
#include <string>

namespace Moly
{
    class MOLY_API Scene
    {
    public:
        Scene(std::string _name = "Default Scene");
        ~Scene() = default;

        std::shared_ptr<Entity> primaryCam = nullptr;

        std::shared_ptr<Entity> createEntity(std::string name);
        std::vector<std::shared_ptr<Entity>> GetEntities() const;
        
        void SetPrimaryCam(std::shared_ptr<Entity> _cam);
        void AddSceneLight(std::shared_ptr<Entity> _light);
        void Update();

    private:
        Entity::EntityID nextID = 0;
        std::string name;

        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::shared_ptr<Entity>> lights;
        
        Renderer renderer;

    };
}