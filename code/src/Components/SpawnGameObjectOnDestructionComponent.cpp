#include "SpawnGameObjectOnDestructionComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "Component.h"
#include "GameStates/GamePlayState.h"
#include "TransformComponent.h"

SpawnGameObjectOnDestructionComponent::SpawnGameObjectOnDestructionComponent(GameObjectRef owner, King::Engine& engine,
                                                                             const std::string& gameObjectTemplateId,
                                                                             const glm::vec2& offset)
    : Component(owner, engine), mGameObjectTemplateId(gameObjectTemplateId), mOffset(offset)
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
    std::string gameObjectTemplateId;
    if(auto it = parameters.find("gameObjectTemplateId"); it != std::end(parameters))
    {
        gameObjectTemplateId = it->second;
    }

    glm::vec2 offset;
    if(auto it = parameters.find("offsetX"); it != std::end(parameters))
    {
        offset.x = std::stof(it->second);
    }

    if(auto it = parameters.find("offsetY"); it != std::end(parameters))
    {
        offset.y = std::stof(it->second);
    }

    return std::shared_ptr<Component>(
        new SpawnGameObjectOnDestructionComponent(owner, engine, gameObjectTemplateId, offset));
}

void SpawnGameObjectOnDestructionComponent::OnCreate()
{
    mTransformComponent = GetOwnerComponent<TransformComponent>();
}

void SpawnGameObjectOnDestructionComponent::OnDestroyed()
{
    std::shared_ptr<GameObject> owner = mOwnerRef.lock();

    if(owner == nullptr)
    {
        return;
    }

    std::shared_ptr<GameObject> newGameObject = GameObject::MakeGameObject(owner->GameState(), mGameObjectTemplateId);
    std::shared_ptr<TransformComponent> transformComponent = mTransformComponent.lock();

    if(newGameObject == nullptr || transformComponent == nullptr)
    {
        return;
    }
    std::shared_ptr<TransformComponent> newTransformComponent = newGameObject->FindComponent<TransformComponent>();

    if(newTransformComponent == nullptr)
    {
        return;
    }

    newTransformComponent->SetPosition(transformComponent->GetPosition() + mOffset);

    owner->GameState().AddGameObject(newGameObject);
}
