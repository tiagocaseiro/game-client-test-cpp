#include "PowerUpComponent.h"

#include <king/Engine.h>

#include "DestroyOnGameStateReset.h"

const std::string& DestroyOnGameStateReset::ID()
{
    static const std::string id = "DestroyOnGameStateReset";
    return id;
}

ComponentShared DestroyOnGameStateReset::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& /*parameters*/)
{

    return std::shared_ptr<Component>(new DestroyOnGameStateReset(owner, engine));
}
