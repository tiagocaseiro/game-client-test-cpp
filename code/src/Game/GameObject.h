#pragma once

#include <memory>
#include <vector>

class Component;

class GameObject
{
public:
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, std::weak_ptr<T>> FindComponent()
    {
        for(std::shared_ptr<Component>& component : mComponents)
        {
            if(auto derived = std::dynamic_pointer_cast<T>(component))
            {
                return derived;
            }
        }
        return std::weak_ptr<T>();
    }

    void AddComponent(std::shared_ptr<Component>);

    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, bool> HasComponent()
    {
        std::weak_ptr<Component> componentRef = FindComponent<T>();

        return componentRef.expired() == false;
    }

    const std::vector<std::shared_ptr<Component>> Components() const;

private:
    GameObject() = default;

    std::vector<std::shared_ptr<Component>> mComponents;

    friend class LevelLoader;
};

using GameObjectRef    = std::weak_ptr<GameObject>;
using GameObjectShared = std::shared_ptr<GameObject>;