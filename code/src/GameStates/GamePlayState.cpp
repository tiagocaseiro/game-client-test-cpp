// (C) king.com Ltd 2021
#include "GamePlayState.h"
#include "Levels/LevelLoader.h"

#include <fstream>
#include <iostream>

#include <SDL.h>

#include "Components/CollisionComponent.h"
#include "Components/Component.h"
#include "Components/HealthComponent.h"
#include "Components/HitDamageComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

ComponentsInitData sComponentsInitData;

// clang-format off
std::unordered_map<std::string, ComponentInternalInitFunc> sComponentTypesInitFuncs = {
    Component::InitData<SpriteComponent>(),          
    Component::InitData<HealthComponent>(),
    Component::InitData<TransformComponent>(),       
    Component::InitData<CollisionBoxComponent>(),
    //Component::InitData<CollisionCircleComponent>()
    Component::InitData<HitDamageComponent>(),

};
// clang-format on

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

void ReadGameConfig(King::Engine& engine)
{
    std::ifstream configFile("./assets/Config/GameConfig.txt");
    if(configFile.is_open() == false)
    {
        return;
    }
    ComponentInternalInitFunc currentComponentInternalInitFunc     = nullptr;
    std::vector<ComponentInitFunc>* currentComponentsInitFuncs     = nullptr;
    std::unordered_map<std::string, std::string> currentParameters = {};

    std::string line;
    while(std::getline(configFile, line))
    {
        if(StartsWith(line, "id="))
        {
            currentComponentInternalInitFunc = nullptr;
            currentComponentsInitFuncs       = nullptr;
            currentParameters.clear();
            auto it = sComponentsInitData.emplace(line.substr(3), std::vector<ComponentInitFunc>{});

            const bool emplaced = it.second;
            if(emplaced)
            {
                currentComponentsInitFuncs = &it.first->second;
            }
            continue;
        }

        if(StartsWith(line, "componentStart=") && currentComponentsInitFuncs)
        {
            currentComponentInternalInitFunc = nullptr;
            std::string componentName        = line.substr(15);
            auto it                          = sComponentTypesInitFuncs.find(componentName);
            if(it != std::end(sComponentTypesInitFuncs))
            {
                ComponentInternalInitFunc initFunc = it->second;
                currentComponentInternalInitFunc   = initFunc;
                continue;
            }

            continue;
        }

        if(StartsWith(line, "componentEnd=") && currentComponentsInitFuncs)
        {
            currentComponentsInitFuncs->emplace_back([currentParameters, currentComponentInternalInitFunc,
                                                      &engine](GameObjectRef owner) -> std::shared_ptr<Component> {
                if(currentComponentInternalInitFunc)
                {
                    return currentComponentInternalInitFunc(owner, engine, currentParameters);
                }
                return nullptr;
            });

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
    ReadGameConfig(mEngine);

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

    mLevel = LevelLoader::LoadLevel(mLevelFilename, mEngine, sComponentsInitData, [this](int score) {
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
            mLevel->DestroyAllBricks();
        }

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

    mEngine.Write(title.c_str(), panelX, y);
    y += 24;
    mEngine.Render(mTextFrameTx, panelX, y, 0, 4);
    y += 16;
    mEngine.Write(text.c_str(), textX, y);
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
