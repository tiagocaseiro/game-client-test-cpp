#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Game/GameObject.h"

class Component;

using ComponentInternalInitFunc = std::function<std::shared_ptr<Component>(
    GameObjectRef, King::Engine&, const std::unordered_map<std::string, std::string>&)>;
using ComponentInitFunc         = std::function<std::shared_ptr<Component>(GameObjectRef)>;
using ComponentsInitData        = std::unordered_map<std::string, std::vector<ComponentInitFunc>>;
using ComponentShared           = std::shared_ptr<Component>;

const std::unordered_map<std::string, ComponentInternalInitFunc>& GetComponentInitFuncs();