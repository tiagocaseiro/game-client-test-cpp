#include "ScoreComponent.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include "CollisionComponent.h"
#include "HealthComponent.h"

const std::string& ScoreComponent::ID()
{
    static const std::string id = "ScoreComponent";
    return id;
}

ComponentShared ScoreComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                              const std::unordered_map<std::string, std::string>& parameters)
{
    int score = DefaultScore();
    if(auto it = parameters.find("score"); it != std::end(parameters))
    {
        score = std::stoi(it->second);
    }
    return std::shared_ptr<Component>(new ScoreComponent(owner, engine, score));
}

int ScoreComponent::DefaultScore()
{
    return 10;
}

int ScoreComponent::ScoreValue() const
{
    return mScore;
}

ScoreComponent::ScoreComponent(GameObjectRef owner, King::Engine& engine, const int score)
    : Component(owner, engine), mScore(score)
{
}
