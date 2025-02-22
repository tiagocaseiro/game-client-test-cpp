// (C) king.com Ltd 2021
#include "GamePlayState.h"
#include "Levels/LevelLoader.h"

#include <fstream>
#include <iostream>

#include <SDL.h>

#include "Components/CollisionComponent.h"
#include "Components/Component.h"
#include "Components/DamageOnCollisionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreOnCollisionComponent.h"
#include "Components/ScoreOnDestructionComponent.h"
#include "Components/SpawnGameObjectOnDestructionComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

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
      mScore(0)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

void ReadSerializedGameTemplateData(King::Engine& engine, const std::string& filePath)
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

            std::optional<const GameObjectTemplate> parentGameObjectTemplateOpt =
                GameObject::FindGameObjectTemplate(parentGameObjectTemplateId);

            if(parentGameObjectTemplateOpt.has_value() == false)
            {
                continue;
            }

            for(const auto& [parentComponentId, parentComponentInitFunc] : *parentGameObjectTemplateOpt)
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
    ReadSerializedGameTemplateData(mEngine, "./assets/Config/BrickGameObjectTemplates.txt");
    ReadSerializedGameTemplateData(mEngine, "./assets/Config/PowerUpGameObjectTemplates.txt");

    mNumBallsLeft = 3;
    mLevelClear   = false;

    mEngine.GetCollisionWorld().ClearAll();

    mPaddle = std::make_unique<Paddle>(mEngine);
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-10, -20), glm::vec2(1044, 20), 1 << 1, 1 << 2));
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-20, 0), glm::vec2(20, 2000), 1 << 1, 1 << 2));
    mColliders.push_back(
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(1024, 0), glm::vec2(20, 2000), 1 << 1, 1 << 2));

    mIdOfBottomCollider =
        mEngine.GetCollisionWorld().AddBoxCollider(glm::vec2(-10, 1024), glm::vec2(1044, 20), 1 << 1, 1 << 2);
    mColliders.push_back(mIdOfBottomCollider);

    mLevel = LevelLoader::LoadLevel(mLevelFilename, mEngine, [this](int score) {
        mScore += score;
    });

    mBGTx = mEngine.LoadTexture(mLevel->Background().c_str());

    mBall = std::make_unique<Ball>(mEngine);
    mBall->SetPaddle(mPaddle.get());

    ResetPaddleAndBall();
    mEngine.SetCursorVisible(false);
}

void GamePlayState::ResetPaddleAndBall()
{
    mPaddle->ResetPosition();
    mBall->SetPos(10, 750);
    mBall->SetStatic(true);
    mBall->SetVelocity(glm::vec2(400, -400));
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
            mLevel->DebugDestroyAllBricks();
        }

        static bool canDebugHit = true;
        if(mEngine.GetKeyDown(SDLK_1) && canDebugHit)
        {
            mLevel->DebugDamageFirstBrick();
        }
        canDebugHit = mEngine.GetKeyUp(SDLK_1);

        if(mEngine.GetMouseButtonPressed())
        {
            mBall->SetStatic(false);
        }

        if(mLevel->NumBricksLeft() == 0)
        {
            mLevelClear = true;
            mBall->SetLevelClear(true);
        }
    }
    mPaddle->Update();
    mBall->Update();
    mLevel->Update();
}

void GamePlayState::Render()
{
    mEngine.Render(mBGTx, -128, 0, 0, 4.0f);

    RenderUI();

    mPaddle->Render();
    mBall->Render();
    mLevel->Render();

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
}
