#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Game/GameObject.h"

#include "ComponentInit.h"

namespace King
{
    class Engine;
}

class Component : public std::enable_shared_from_this<Component>
{
public:
    Component(GameObjectRef owner, King::Engine& engine);

    virtual ~Component() = default;

    // Called when GameObject's initial set of Components is initialized
    virtual void OnCreate();
    // Called every GameState Render call
    virtual void Render();
    // Called every GameState Update call
    virtual void Update();
    // Called right before destructor is called
    virtual void OnDestroyed();
    // Called when a new sibling component is Added to GameObject
    virtual void OnComponentAdded(std::shared_ptr<Component>);

    template <typename T>
    static std::pair<std::string, ComponentInitFunc> InitData()
    {
        static_assert(std::is_base_of_v<Component, T>, "Type T must be derived from Component class");

        return std::pair(T::ID(), T::MakeComponent);
    }

    template <typename T>
    std::shared_ptr<T> GetOwnerComponent()
    {
        if(mOwnerRef.expired())
        {
            return {};
        }

        return mOwnerRef.lock()->FindComponent<T>();
    }

    template <typename T>
    std::shared_ptr<T> Cast()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

protected:
    GameObjectRef mOwnerRef;
    King::Engine& mEngine;
};
