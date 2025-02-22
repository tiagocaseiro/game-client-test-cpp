#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <king/Engine.h>

#include "Components/ComponentInit.h"

class Component;
class Level;
class GameObject;

namespace King
{
    class Engine;
}

using GameObjectRef    = std::weak_ptr<GameObject>;
using GameObjectShared = std::shared_ptr<GameObject>;

class GameObject final : public std::enable_shared_from_this<GameObject>
{
public:
    static GameObjectShared MakeGameObject(Level& level, const std::string gameObjectTemplateId);

    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> FindComponent()

    {
        for(std::shared_ptr<Component>& component : mComponents)
        {
            if(component == nullptr)
            {
                continue;
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

    static void AddComponentInitFunc(
        const std::string& gameObjectTemplateId, const std::string& componentId,
        const std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>)>& componentInitFunc);

    static std::optional<const GameObjectTemplate> FindGameObjectTemplate(const std::string& gameObjectTemplateId);

private:
    GameObject(Level& level);

    std::vector<std::shared_ptr<Component>> mComponents;

    Level& mLevel;
};
