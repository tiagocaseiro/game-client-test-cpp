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

// Map that holds key/value pairs of parameterName and parameterValue for Component initialization
using ComponentParameters = std::unordered_map<std::string, std::string>;

// Function that creates an instance of a Component
using ComponentInitFunc =
    std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>, King::Engine&, const ComponentParameters&)>;

// Map that holds key/value pairs of ComponentId and the function that creates a instance of said Component with the
// specific set of parameters for that GameObjectTemplate
using GameObjectTemplate =
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>)>>;

// Map that holds key/value pairs of GameObjectTemplateId and GameObjectTemplate, which holds all the necessary
// functions to initialize all of its Components
using GameObjectTemplates = std::unordered_map<std::string, GameObjectTemplate>;

using ComponentShared = std::shared_ptr<Component>;

// Returns list of all Components and their initialization functions. Basically their functions that are able to
// instantiate a Component out of ComponentParameters
const std::unordered_map<std::string, ComponentInitFunc>& GetComponentInitFuncs();