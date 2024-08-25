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

        std::shared_ptr<Entity> createEntity();

        void Update();

    private:
        std::string name;

        std::vector<std::shared_ptr<Entity>> entities;
        Entity::EntityID nextID = 0;
        Renderer renderer;

    };
}