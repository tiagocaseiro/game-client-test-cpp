#pragma once

#include <memory>
#include <vector>

#include <king/Engine.h>

class Component;
class Level;

class GameObject final : public std::enable_shared_from_this<GameObject>
{
public:
    //~GameObject();
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> FindComponent()
    {
        for(std::shared_ptr<Component>& component : mComponents)
        {
            if(component == nullptr)
            {
                return nullptr;
            }

            if(auto derived = component->Cast<T>())
            {
                return derived;
            }
        }
        return nullptr;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, bool> HasComponent()
    {
        return FindComponent<T>() != nullptr;
    }

    void AddComponent(std::shared_ptr<Component>);
    void MarkForDeath();

    Level& GameLevel();

    const std::vector<std::shared_ptr<Component>> Components() const;

private:
    GameObject(Level& level);

    std::vector<std::shared_ptr<Component>> mComponents;

    friend class LevelLoader;

    Level& mLevel;
};

using GameObjectRef    = std::weak_ptr<GameObject>;
using GameObjectShared = std::shared_ptr<GameObject>;