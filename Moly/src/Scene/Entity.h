#pragma once

#include <Core.h>
#include <Components.h>

#include <string>
#include <memory>
#include <typeindex>

#include <glm/glm.hpp>

namespace Moly
{
    class MOLY_API Entity {
    public:
        using EntityID = std::size_t;

    public:
        Entity(EntityID id) : id(id) {}

        EntityID getID() const { return id; }

        template<typename T>
        void addComponent(std::shared_ptr<T> component) {
            components[std::type_index(typeid(T))] = component;
        }

        template<typename T>
        std::shared_ptr<T> getComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }
    private:
        EntityID id;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

    };
}
