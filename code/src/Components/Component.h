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

    virtual void Render();
    virtual void Update();
    virtual void OnDestroyed();

    template <typename T>
    static std::pair<std::string, ComponentInternalInitFunc> InitData()
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
