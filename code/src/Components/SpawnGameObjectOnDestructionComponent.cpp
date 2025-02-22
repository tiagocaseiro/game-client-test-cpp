#include "SpawnGameObjectOnDestructionComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "Component.h"
#include "Levels/Level.h"
#include "TransformComponent.h"

SpawnGameObjectOnDestructionComponent::SpawnGameObjectOnDestructionComponent(GameObjectRef owner, King::Engine& engine)
    : Component(owner, engine)
{
}

const std::string& SpawnGameObjectOnDestructionComponent::ID()
{
    static const std::string id = "SpawnGameObjectOnDestructionComponent";
    return id;
}

ComponentShared SpawnGameObjectOnDestructionComponent::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& parameters)
{
    return std::shared_ptr<Component>(new SpawnGameObjectOnDestructionComponent(owner, engine));
}

void SpawnGameObjectOnDestructionComponent::OnDestroyed()
{
    if(mOwnerRef.expired())
    {
        return;
    }
}
