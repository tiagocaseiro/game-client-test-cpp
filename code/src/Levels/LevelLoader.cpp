// (C) king.com Ltd 2021
#include "LevelLoader.h"

#include "glm/vec2.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace
{
bool StartsWith(const std::string& text, const std::string& start)
{
    return (text.find(start) == 0);
}

void ParseBlockChar(char c, Level* level, const glm::vec2& brickPos)
{
    if(c == 'r')
    {
        // Red block
        level->AddBrick(brickPos, Brick::BRICK_RED);
    }
    else if(c == 'g')
    {
        level->AddBrick(brickPos, Brick::BRICK_GREEN);
    }
    else if(c == 'b')
    {
        level->AddBrick(brickPos, Brick::BRICK_BLUE);
    }
    else if(c == 'o')
    {
        level->AddBrick(brickPos, Brick::BRICK_ORANGE);
    }
    else if(c == 'v')
    {
        level->AddBrick(brickPos, Brick::BRICK_GLASS);
    }
    else if(c == 'c')
    {
        level->AddBrick(brickPos, Brick::BRICK_GREY, 2);
    }
    else if(c == 's')
    {
        level->AddBrick(brickPos, Brick::BRICK_SOLID);
    }
    else if(c > '0' && c < '8')
    {
        int hitPoints = c - '0';
        level->AddBrick(brickPos, Brick::BRICK_COUNTER, hitPoints);
    }
    else if(c == 'p')
    {
        assert(false && "Unknown type of brick!");
    }
}
} // namespace

std::unique_ptr<Level> LevelLoader::LoadLevel(const std::string& levelName, King::Engine& engine,
                                              const ComponentsInitData& componentsInitData,
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

            if(StartsWith(line, "paddles="))
            {
                std::string id;
                std::getline(levelFile, id);

                if(std::shared_ptr<GameObject> gameObject = std::shared_ptr<GameObject>(new GameObject))
                {
                    for(const ComponentInitFunc& initFunc : componentsInitData.at(id))
                    {
                        gameObject->AddComponent(initFunc(gameObject));
                    }
                }
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
                        brickPos.x = float(x * kBrickWidth);
                        brickPos.y = float(y * kBrickHeight);

                        ParseBlockChar(row[0], level.get(), brickPos);
                        x++;
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
