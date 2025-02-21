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

class Component;

using ComponentInternalInitFunc = std::function<std::shared_ptr<Component>(
    GameObjectRef, King::Engine&, const std::unordered_map<std::string, std::string>&)>;
using ComponentInitFunc         = std::function<std::shared_ptr<Component>(GameObjectRef)>;
using ComponentsInitData        = std::unordered_map<std::string, std::vector<ComponentInitFunc>>;
using ComponentShared           = std::shared_ptr<Component>;

class Component
{
public:
    Component(GameObjectRef owner, King::Engine& engine);

    virtual ~Component() = default;

    virtual void Render();
    virtual void Update();

    template <typename T>
    static std::pair<std::string, ComponentInternalInitFunc> InitData()
    {
        static_assert(std::is_base_of_v<Component, T>, "Type T must be derived from Component class");

        return std::pair(T::ID(), T::MakeComponent);
    }

protected:
    const GameObjectRef mOwnerRef;
    King::Engine& mEngine;
};
