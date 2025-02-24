// (C) king.com Ltd 2021
#include <king/Engine.h>
#include <king/Updater.h>

#include "GameStates/GameEndState.h"
#include "GameStates/GameMainMenuState.h"
#include "GameStates/GamePlayState.h"
#include "GameStates/GameState.h"

#include <vector>

#include <memory>

//**********************************************************************

class ExampleGame : public King::Updater
{
public:
    ExampleGame() : mEngine("./assets"), mCurrentGameState(nullptr)
    {

        mMainMenuState = std::make_unique<GameMainMenuState>(mEngine, [&]() {
            // Necessary to have this called before resetting the score, otherwise the destruction of the bricks will
            // increase the score (maybe we should have a flag that indicates that the game has started?)
            mPlayState->ClearRemovedObjects();
            mPlayState->ResetScore();
            mPlayState->SetLevelFilename("./assets/Levels/Level1.txt");
            SetGameState(mPlayState.get());
        });

        mPlayState = std::make_unique<GamePlayState>(mEngine, [&](bool won, int score) {
            mGameEndState->SetGameResult(won, score);
            SetGameState(mGameEndState.get());
        });

        mGameEndState = std::make_unique<GameEndState>(mEngine, [&]() {
            SetGameState(mMainMenuState.get());
        });

        SetGameState(mMainMenuState.get());
    }

    void Start()
    {
        mEngine.Start(*this);
    }

    void Update()
    {
        if(!mCurrentGameState)
        {
            return;
        }

        mCurrentGameState->Update();
        mCurrentGameState->Render();
    }

    void SetGameState(GameState* gameState)
    {
        assert(gameState);

        if(mCurrentGameState)
        {
            mCurrentGameState->End();
        }

        mCurrentGameState = gameState;
        mCurrentGameState->Start();
    }

private:
    King::Engine mEngine;
    // States
    std::unique_ptr<GameMainMenuState> mMainMenuState;
    std::unique_ptr<GamePlayState> mPlayState;
    std::unique_ptr<GameEndState> mGameEndState;

    GameState* mCurrentGameState;
};

//**********************************************************************

int main(int /*argc*/, char* /*argv*/[])
{
    ExampleGame game;
    game.Start();

    return 0;
}
