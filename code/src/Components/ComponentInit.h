#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class Component;
class GameObject;

namespace King
{
    class Engine;
}

using ComponentParameters = std::unordered_map<std::string, std::string>;
using ComponentInitFunc =
    std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>, King::Engine&, const ComponentParameters&)>;
using GameObjectTemplate  = std::vector<std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>)>>;
using GameObjectTemplates = std::unordered_map<std::string, GameObjectTemplate>;

using ComponentShared = std::shared_ptr<Component>;

const std::unordered_map<std::string, ComponentInitFunc>& GetComponentInitFuncs();