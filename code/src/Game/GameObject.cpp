#include "GameObject.h"

#include <iostream>

#include <king/Engine.h>

#include "Components/Component.h"
#include "Components/ComponentInit.h"
#include "GameStates/GamePlayState.h"

static GameObjectTemplates sGameObjectTemplates;

GameObjectShared GameObject::MakeGameObject(GamePlayState& gameState, const std::string gameObjectTemplateId)
{
    GameObjectShared gameObject = std::shared_ptr<GameObject>(new GameObject(gameObjectTemplateId, gameState));

    if(gameObject == nullptr)
    {
        return nullptr;
    }

    const GameObjectTemplate* gameObjectTemplate = FindGameObjectTemplate(gameObjectTemplateId);

    if(gameObjectTemplate == nullptr)
    {
        return nullptr;
    }

    for(const auto& componentInitData : *gameObjectTemplate)
    {
        const std::function<ComponentShared(GameObjectRef)>& componentInitFunc = componentInitData.second;
        gameObject->AddComponent(componentInitFunc(gameObject));
    }

    for(const ComponentShared& component : gameObject->mComponents)
    {
        if(component)
        {
            component->OnCreate();
        }
    }

    return gameObject;
}

GameObject::GameObject(const std::string& debugGameObjectTemplateId, GamePlayState& gameState)
    : mDebugGameObjectTemplateId(debugGameObjectTemplateId), mGameState(gameState)
{
}

void GameObject::AddComponent(std::shared_ptr<Component> newComponent)
{
    mComponents.push_back(newComponent);

    for(const std::shared_ptr<Component>& component : mComponents)
    {
        if(component)
        {
            component->OnComponentAdded(newComponent);
        }
    }
}

const std::vector<std::shared_ptr<Component>> GameObject::Components() const
{
    return mComponents;
}

void GameObject::AddComponentInitFunc(
    const std::string& gameObjectTemplateId, const std::string& componentId,
    const std::function<std::shared_ptr<Component>(std::weak_ptr<GameObject>)>& componentInitFunc)
{
    GameObjectTemplate& gameObjectTemplate = sGameObjectTemplates[gameObjectTemplateId];

    auto it = gameObjectTemplate.find(componentId);
    if(it == std::end(gameObjectTemplate))
    {
        gameObjectTemplate.emplace(componentId, componentInitFunc);
        return;
    }
    it->second = componentInitFunc;
}

const GameObjectTemplate* GameObject::FindGameObjectTemplate(const std::string& id)
{
    auto it = sGameObjectTemplates.find(id);

    if(it == std::end(sGameObjectTemplates))
    {
        return nullptr;
    }

    return &it->second;
}

void GameObject::MarkForDeath()
{
    mGameState.MarkForDeath(shared_from_this());
}

GamePlayState& GameObject::GameState()
{
    return mGameState;
}
