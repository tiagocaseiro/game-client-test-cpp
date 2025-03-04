// (C) king.com Ltd 2021
#include "LevelLoader.h"

#include "glm/vec2.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "Components/CollisionComponent.h"
#include "Components/TransformComponent.h"
#include "GameStates/GamePlayState.h"

static bool StartsWith(const std::string& text, const std::string& start)
{
    return (text.find(start) == 0);
}

std::unique_ptr<Level> LevelLoader::LoadLevel(const std::string& levelName, King::Engine& engine,
                                              GamePlayState& gameState)
{
    std::unique_ptr<Level> level = std::make_unique<Level>(engine);

    std::string levelTitle      = "Untitled";
    std::string levelBackground = "Background-01.png";
    std::string nextLevelFilename;

    std::ifstream levelFile(levelName);

    int x = 0;
    int y = 0;
    glm::vec2 brickPos;

    if(levelFile.is_open())
    {
        std::string line;
        while(std::getline(levelFile, line))
        {
            if(StartsWith(line, "Title="))
            {
                levelTitle = line.substr(6);
                continue;
            }

            if(StartsWith(line, "Background="))
            {
                levelBackground = line.substr(11);
                continue;
            }

            if(StartsWith(line, "NextLevelFile="))
            {
                nextLevelFilename = line.substr(14);
                continue;
            }

            if(StartsWith(line, "tiles="))
            {
                // Tiles must be the last thing in the level file
                while(std::getline(levelFile, line))
                {
                    std::istringstream lineStream(line);
                    std::string row;
                    while(std::getline(lineStream, row, ','))
                    {
                        gameState.AddGameObject(LoadBrick(row, brickPos, x, y, gameState));
                    }
                    x = 0;
                    y++;
                }
            }
        }
        levelFile.close();
    }

    level->SetName(levelTitle);
    level->SetBackground(levelBackground);
    level->SetNextLevelFilename(nextLevelFilename);

    return level;
}

std::shared_ptr<GameObject> LevelLoader::LoadBrick(const std::string& row, glm::vec2& brickPos, int& x, int& y,
                                                   GamePlayState& gameState)
{
    brickPos.x = float(x * kBrickWidth);
    brickPos.y = float(y * kBrickHeight);

    std::string brickType = row.substr(0, 1);

    std::shared_ptr<GameObject> brick = GameObject::MakeGameObject(gameState, brickType);
    if(brick != nullptr)
    {
        // Set brick position
        if(std::shared_ptr<TransformComponent> transformComponent = brick->FindComponent<TransformComponent>())
        {
            transformComponent->SetPosition(brickPos);
        }

        // Set brick collision box data
        if(std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = brick->FindComponent<CollisionBoxComponent>())
        {
            collisionBoxComponent->SetPosition(brickPos);
            // This step might be pointless but now I don't want to touch this right before submitting this exercise
            collisionBoxComponent->SetDimensions({kBrickWidth, kBrickHeight});
        }
    }
    x++;
    return brick;
}