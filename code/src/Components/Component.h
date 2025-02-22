#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Game/GameObject.h"

namespace King
{
class Engine;
}

class Level;
class Component;

using ComponentInternalInitFunc = std::function<std::shared_ptr<Component>(
    GameObjectRef, King::Engine&, const std::unordered_map<std::string, std::string>&)>;
using ComponentInitFunc         = std::function<std::shared_ptr<Component>(GameObjectRef)>;
using ComponentsInitData        = std::unordered_map<std::string, std::vector<ComponentInitFunc>>;
using ComponentShared           = std::shared_ptr<Component>;

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
