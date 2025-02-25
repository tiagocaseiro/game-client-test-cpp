// (C) king.com Ltd 2021
#include "GamePlayState.h"
#include "Levels/LevelLoader.h"

#include <fstream>
#include <iostream>

#include <SDL.h>

#include "Components/CollisionComponent.h"
#include "Components/Component.h"
#include "Components/DamageOnCollisionComponent.h"
#include "Components/DestroyOnGameStateReset.h"
#include "Components/HealthComponent.h"
#include "Components/PowerUpComponent.h"
#include "Components/ScoreOnCollisionComponent.h"
#include "Components/ScoreOnDestructionComponent.h"
#include "Components/SpawnGameObjectOnDestructionComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

#include "Game/GameObject.h"

bool StartsWith(const std::string& text, const std::string& start)
{
    return (text.find(start) == 0);
}

GamePlayState::GamePlayState(King::Engine& engine, GameEndedFunction gameEndedFunction)
    : mEngine(engine),
      mGameEndedFunction(gameEndedFunction),
      mBGTx(mEngine.LoadTexture("Background-01.png")),
      mPanelTx(mEngine.LoadTexture("Panel.png")),
      mTextFrameTx(mEngine.LoadTexture("Text-frame.png")),
      mScore(0),
      mCanScore(false)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

void ReadSerializedGameObjectTemplates(King::Engine& engine, const std::string& filePath)
{
    std::ifstream configFile(filePath);

    if(configFile.is_open() == false)
    {
        return;
    }
    ComponentInitFunc currentComponentInitFunc;
    std::string componentId;
    std::unordered_map<std::string, std::string> currentParameters;
    std::string currentGameObjectTemplateId;
    std::string line;
    while(std::getline(configFile, line))
    {
        if(StartsWith(line, "id="))
        {
            currentComponentInitFunc = nullptr;
            currentParameters.clear();
            currentGameObjectTemplateId = line.substr(3);
            continue;
        }

        if(StartsWith(line, "parent="))
        {
            std::string parentGameObjectTemplateId = line.substr(7);

            const GameObjectTemplate* parentGameObjectTemplate =
                GameObject::FindGameObjectTemplate(parentGameObjectTemplateId);

            if(parentGameObjectTemplate == nullptr)
            {
                continue;
            }

            for(const auto& [parentComponentId, parentComponentInitFunc] : *parentGameObjectTemplate)
            {
                GameObject::AddComponentInitFunc(currentGameObjectTemplateId, parentComponentId,
                                                 parentComponentInitFunc);
            }
            continue;
        }

        if(StartsWith(line, "componentStart="))
        {
            currentComponentInitFunc = nullptr;
            componentId              = line.substr(15);
            auto it                  = GetComponentInitFuncs().find(componentId);
            if(it != std::end(GetComponentInitFuncs()))
            {
                ComponentInitFunc initFunc = it->second;
                currentComponentInitFunc   = initFunc;
                continue;
            }

            continue;
        }

        if(StartsWith(line, "componentEnd="))
        {
            const auto initFunc = [currentParameters, currentComponentInitFunc,
                                   &engine](GameObjectRef owner) -> std::shared_ptr<Component> {
                if(currentComponentInitFunc)
                {
                    return currentComponentInitFunc(owner, engine, currentParameters);
                }
                return nullptr;
            };

            GameObject::AddComponentInitFunc(currentGameObjectTemplateId, componentId, initFunc);
            currentParameters.clear();
            continue;
        }

        auto parameterSeparatorIndex = line.find_first_of('=');
        if(parameterSeparatorIndex != std::string::npos)
        {
            auto name = line.substr(0, parameterSeparatorIndex);
            parameterSeparatorIndex++;
            auto value = line.substr(parameterSeparatorIndex);
            currentParameters.emplace(name, value);
            continue;
        }
    }
}

void GamePlayState::Start()
{
    // mCanScore flag prevents score updates from happening when deleting objects from previous session that score when
    // destroyed
    mCanScore = false;

    while(mGameObjects.empty() == false || mGameObjectsToDelete.empty() == false)
    {
        // ClearRemovedObjects destroys GameObjects that can lead to the creation of other GameObjects (due to holding a
        // SpawnGameObjectOnDestructionComponent)
        // So, we might need to run multiple iterations of this to make sure that all subsequent GameObjects that are
        // created are deleted
        // Destroying these objects is also necessary because of after this will be clearing all colliders and some of
        // these objects would be holding RigidBodies with no matching collider, which would lead to a crash
        MarkAllGameObjectsForRemove();
        ClearRemovedObjects();
    }

    // TODO: Move this to a spot where it's only read once and not before every level
    ReadSerializedGameObjectTemplates(mEngine, "./assets/GameObjectTemplates/BrickGameObjectTemplates.txt");
    ReadSerializedGameObjectTemplates(mEngine, "./assets/GameObjectTemplates/PowerUpGameObjectTemplates.txt");

    mNumBallsLeft = 3;
    mLevelClear   = false;

    // This ClearAll gave me a lot of trouble
    // The responsibility of clearing collision data should be delegated instead of centralized like this
    mEngine.GetCollisionWorld().ClearAll();

    mPaddle = std::make_unique<Paddle>(mEngine);
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-10, -20), glm::vec2(1044, 20), 1 << 1, 1 << 2));
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-20, 0), glm::vec2(20, 2000), 1 << 1, 1 << 2));
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(1024, 0), glm::vec2(20, 2000), 1 << 1, 1 << 2));

    mIdOfBottomCollider =
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-10, 1024), glm::vec2(1044, 20), 1 << 1, 1 << 2 | 1 << 4);
    mColliders.push_back(mIdOfBottomCollider);

    mLevel = LevelLoader::LoadLevel(mLevelFilename, mEngine, *this);

    mBGTx = mEngine.LoadTexture(mLevel->Background().c_str());

    mBall = std::make_unique<Ball>(mEngine);
    mBall->SetPaddle(mPaddle.get());

    ResetPaddleAndBall();
    mEngine.SetCursorVisible(false);

    mCanScore = true;
}

void GamePlayState::ResetPaddleAndBall()
{
    mPaddle->Reset();

    for(const std::shared_ptr<GameObject>& gameObject : mGameObjects)
    {
        // Clean screen of power-ups dropping
        if(gameObject && gameObject->HasComponent<DestroyOnGameStateReset>())
        {
            gameObject->MarkForDeath();
        }
    }
    mBall->SetPos(10, 750);
    mBall->SetStatic(true);
    mBall->SetVelocity(glm::vec2(400, -400));
}

void GamePlayState::UpdateGameObjects()
{
    // For safety, iterate through a copy of gameObjects because the original can be resized
    std::vector<GameObjectShared> gameObjects = mGameObjects;
    for(auto it = std::begin(mGameObjects); it != std::end(mGameObjects);)
    {
        std::shared_ptr<GameObject> gameObject = *it;
        if(gameObject == nullptr)
        {
            it = mGameObjects.erase(it);
            continue;
        }

        if(mGameObjectsToDelete.find(gameObject) != std::end(mGameObjectsToDelete))
        {
            it = mGameObjects.erase(it);
            continue;
        }

        for(const ComponentShared& component : gameObject->Components())
        {
            if(component)
            {
                component->Update();
            }
        }
        it++;
    }

    ClearRemovedObjects();
}

void GamePlayState::Update()
{
    if(mLevelClear)
    {
        if(mEngine.GetMouseButtonDown())
        {
            std::string nextLevelFilename = mLevel->NextLevelFilename();
            if(nextLevelFilename.size() == 0)
            {
                // We won the game!
                mGameEndedFunction(true, mScore);
            }
            else
            {
                SetLevelFilename(nextLevelFilename);
                Start();
            }
        }
    }
    else
    {
        // Cheat!
        if(mEngine.GetKeyDown(SDLK_w))
        {
            // Call regular marking of objects for removal so they follow their normal OnDestroyed procedures
            MarkAllGameObjectsForRemove();
        }

        // Flag is necessary to handle it only as an OnRelease of 1 key
        static bool canDebugHit = true;
        if(mEngine.GetKeyDown(SDLK_1) && canDebugHit)
        {
            // Simulate hitting first available brick
            DebugHitBrick();
        }
        canDebugHit = mEngine.GetKeyUp(SDLK_1);

        // Hit first brick while holding down 2 key
        if(mEngine.GetKeyDown(SDLK_2))
        {
            // Simulate hitting first available brick
            DebugHitBrick();
        }

        if(mEngine.GetMouseButtonPressed())
        {
            mBall->SetStatic(false);
        }

        if(NumBricks() == 0)
        {
            mLevelClear = true;
            mBall->SetLevelClear(true);
        }
    }

    mPaddle->Update();
    mBall->Update();
    UpdateGameObjects();
}

void GamePlayState::Render()
{
    mEngine.Render(mBGTx, -128, 0, 0, 4.0f);

    RenderUI();

    mPaddle->Render();
    mBall->Render();
    RenderGameObjects();
    if(mLevelClear)
    {
        mEngine.WriteCentered("LEVEL CLEAR!", 512, 480, 4.0f);
        mEngine.WriteCentered("Click to go to next level", 512, 540, 2.0f);
    }
}

void GamePlayState::ResetScore()
{
    mScore = 0;
}

void GamePlayState::RenderUI()
{
    mEngine.Render(mPanelTx, 1024, 0, 0, 4.0f);

    RenderUIIndicator(20, "Current Level", mLevel->Name());
    RenderUIIndicator(120, "Balls", std::to_string(mNumBallsLeft - 1));
    RenderUIIndicator(220, "Score", std::to_string(mScore));
    std::optional<Paddle::PowerUp> timedData = mPaddle->GetPowerUp();
    if(timedData && timedData->timer)
    {
        RenderUIIndicator(320, "Power-Up", std::to_string(static_cast<unsigned int>(std::ceilf(*timedData->timer))));
    }
}

void GamePlayState::RenderUIIndicator(int y, std::string title, std::string text)
{
    const int panelX = 1036;
    const int textX  = panelX + 16;

    mEngine.Write(title.c_str(), panelX, static_cast<float>(y));
    y += 24;
    mEngine.Render(mTextFrameTx, panelX, static_cast<float>(y), 0, 4);
    y += 16;
    mEngine.Write(text.c_str(), textX, static_cast<float>(y));
}

void GamePlayState::OnCollision(int l, int r)
{
    bool hitBottom    = l == mIdOfBottomCollider || r == mIdOfBottomCollider;
    bool oneIsTheBall = l == mBall->ColliderId() || r == mBall->ColliderId();
    if(hitBottom && !mLevelClear && oneIsTheBall)
    {
        if(--mNumBallsLeft == 0)
        {
            mGameEndedFunction(false, mScore);
        }
        else
        {
            ResetPaddleAndBall();
        }
    }
}

void GamePlayState::End()
{
    mEngine.SetCursorVisible(true);
    for(const auto& collider : mColliders)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(collider);
    }
    mColliders.clear();

    // Can't remove game objects immediately because this can be called while iterating CollisionWorld::mListeners
    // which would invalidate pointers in said container and possibly resize it, if new GameObjects are created on
    // destruction
    MarkAllGameObjectsForRemove();
}

void GamePlayState::AddGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    mGameObjects.emplace_back(gameObject);
}

void GamePlayState::MarkForDeath(const std::shared_ptr<GameObject>& gameObject)
{
    mGameObjectsToDelete.emplace(gameObject);
}

void GamePlayState::UpdateScore(int score)
{
    if(mCanScore)
    {
        mScore += score;
    }
}

Paddle* GamePlayState::GetPaddle()
{
    return mPaddle.get();
}

void GamePlayState::RenderGameObjects()
{
    for(const GameObjectShared& gameObject : mGameObjects)
    {
        if(gameObject == nullptr)
        {
            continue;
        }

        for(const ComponentShared& component : gameObject->Components())
        {
            if(component)
            {
                component->Render();
            }
        }
    }
}

void GamePlayState::DebugHitBrick()
{
    if(mGameObjects.empty())
    {
        return;
    }

    for(GameObjectShared& brick : mGameObjects)
    {
        if(brick == nullptr)
        {
            continue;
        }

        std::shared_ptr<CollisionBoxComponent> collisionComponent = brick->FindComponent<CollisionBoxComponent>();
        if(collisionComponent == nullptr)
        {
            continue;
        }

        std::optional<int> colliderId = collisionComponent->ColliderId();
        if(colliderId.has_value() == false)
        {
            continue;
        }

        bool isToReturn = false;

        if(std::shared_ptr<DamageOnCollisionComponent> damageOnCollisionComponent =
               brick->FindComponent<DamageOnCollisionComponent>())
        {
            isToReturn = true;
            damageOnCollisionComponent->OnCollision(*colliderId, *colliderId);
        }

        if(std::shared_ptr<ScoreOnCollisionComponent> scoreOnCollisionComponent =
               brick->FindComponent<ScoreOnCollisionComponent>())
        {
            scoreOnCollisionComponent->OnCollision(*colliderId, *colliderId);
        }
        if(isToReturn)
        {
            return;
        }
    }
}

int GamePlayState::NumBricks() const
{
    // TODO: Have the concept of bricks not be tied down to having a HealthComponent
    return std::count_if(std::begin(mGameObjects), std::end(mGameObjects), [](const GameObjectRef& gameObjectShared) {
        return gameObjectShared.expired() == false && gameObjectShared.lock()->HasComponent<HealthComponent>();
    });
}

void GamePlayState::ClearRemovedObjects()
{
    for(const std::shared_ptr<GameObject>& gameObject : mGameObjectsToDelete)
    {
        if(gameObject == nullptr)
        {
            continue;
        }
        for(const std::shared_ptr<Component>& component : gameObject->Components())
        {
            if(component)
            {
                component->OnDestroyed();
            }
        }
    }
    mGameObjectsToDelete.clear();
}

void GamePlayState::MarkAllGameObjectsForRemove()
{
    for(const std::shared_ptr<GameObject>& gameObject : mGameObjects)
    {
        mGameObjectsToDelete.insert(gameObject);
    }
    mGameObjects.clear();
}
