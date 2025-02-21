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

class Component
{
public:
    Component(GameObjectRef owner, King::Engine& engine);

    virtual ~Component() = default;

private:
    const GameObjectRef mOwner;
    King::Engine& mEngine;
};

using ComponentInternalInitFunc =
    std::function<std::shared_ptr<Component>(GameObjectRef, King::Engine&, const std::vector<std::string>&)>;
using ComponentInitFunc  = std::function<std::shared_ptr<Component>(GameObjectRef)>;
using ComponentsInitData = std::unordered_map<std::string, std::vector<ComponentInitFunc>>;
using ComponentShared    = std::shared_ptr<Component>;