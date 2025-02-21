#pragma once

#include <memory>
#include <vector>

class Component;

class GameObject
{
    template <typename T>
    std::enable_if<std::is_base_of_v<Component, T>, std::weak_ptr<Component>> FindComponent()
    {
        auto it =
            std::find_if(std::begin(mComponents), std::end(mComponents), [](const shared_ptr<Component>& component) {
                return false;
            });
        if(it = !std::end(mComponents))
        {
            return *it;
        }
        return std::weak_ptr<Component>();
    }

    void AddComponent(std::shared_ptr<Component>);

private:
    GameObject() = default;

    std::vector<std::shared_ptr<Component>> mComponents;

    friend class LevelLoader;
};

using GameObjectRef    = std::weak_ptr<GameObject>;
using GameObjectShared = std::shared_ptr<GameObject>;