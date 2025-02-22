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

namespace
{
    bool StartsWith(const std::string& text, const std::string& start)
    {
        return (text.find(start) == 0);
    }
    //
    // void ParseBlockChar(char c, Level* level, const glm::vec2& brickPos)
    //{
    //    if(c == 'r')
    //    {
    //        // Red block
    //        level->AddBrick(brickPos, Brick::BRICK_RED);
    //    }
    //    else if(c == 'g')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_GREEN);
    //    }
    //    else if(c == 'b')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_BLUE);
    //    }
    //    else if(c == 'o')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_ORANGE);
    //    }
    //    else if(c == 'v')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_GLASS);
    //    }
    //    else if(c == 'c')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_GREY, 2);
    //    }
    //    else if(c == 's')
    //    {
    //        level->AddBrick(brickPos, Brick::BRICK_SOLID);
    //    }
    //    else if(c > '0' && c < '8')
    //    {
    //        int hitPoints = c - '0';
    //        level->AddBrick(brickPos, Brick::BRICK_COUNTER, hitPoints);
    //    }
    //    else if(c == 'p')
    //    {
    //        assert(false && "Unknown type of brick!");
    //    }
    //}
} // namespace

std::unique_ptr<Level> LevelLoader::LoadLevel(const std::string& levelName, King::Engine& engine,
                                              const GameObjectTemplates& gameObjectTemplates,
                                              Level::ScoreReportingFunction scoreReportingFunction)
{
    std::unique_ptr<Level> level = std::make_unique<Level>(engine, scoreReportingFunction);
    level->Reset();

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
                        LoadBrick(gameObjectTemplates, row, *level, brickPos, x, y);
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

void LevelLoader::LoadBrick(const GameObjectTemplates& gameObjectTemplates, const std::string& row, Level& level,
                            glm::vec2& brickPos, int& x, int& y)
{
    brickPos.x = float(x * kBrickWidth);
    brickPos.y = float(y * kBrickHeight);

    std::string brickType = row.substr(0, 1);

    auto it = gameObjectTemplates.find(brickType);
    if(it == std::end(gameObjectTemplates))
    {
        x++;
        return;
    }

    std::shared_ptr<GameObject> brick = GameObject::MakeGameObject(level, it->second);
    if(brick != nullptr)
    {
        level.AddBrick(brick);

        // Set brick position
        if(std::shared_ptr<TransformComponent> transformComponent = brick->FindComponent<TransformComponent>())
        {
            transformComponent->SetPosition(brickPos);
        }

        // Set brick collision box data
        if(std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = brick->FindComponent<CollisionBoxComponent>())
        {
            collisionBoxComponent->UpdateData(brickPos, {kBrickWidth, kBrickHeight});
        }
    }
    x++;
}