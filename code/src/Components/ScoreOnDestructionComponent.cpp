#include "ScoreOnDestructionComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "Component.h"
#include "Levels/Level.h"
#include "TransformComponent.h"

ScoreOnDestructionComponent::ScoreOnDestructionComponent(GameObjectRef owner, King::Engine& engine, const int score)
    : Component(owner, engine), mScore(score)
{
}

const std::string& ScoreOnDestructionComponent::ID()
{
    static const std::string id = "ScoreOnDestructionComponent";
    return id;
}

ComponentShared ScoreOnDestructionComponent::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& parameters)
{
    int score = 0;

    if(auto it = parameters.find("score"); it != std::end(parameters))
    {
        score = std::stoi(it->second);
    }

    return std::shared_ptr<Component>(new ScoreOnDestructionComponent(owner, engine, score));
}

void ScoreOnDestructionComponent::OnDestroyed()
{
    if(mOwnerRef.expired())
    {
        return;
    }
    mOwnerRef.lock()->GameLevel().UpdateScore(mScore);
}
