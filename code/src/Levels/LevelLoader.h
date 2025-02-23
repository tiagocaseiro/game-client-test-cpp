// (C) king.com Ltd 2021
#pragma once

#include "Level.h"

#include <string>

#include "Components/Component.h"

class GameObject;
class LevelLoader
{
public:
    static std::unique_ptr<Level> LoadLevel(const std::string& levelName, King::Engine& engine,
                                            GamePlayState& gameState);

private:
    static std::shared_ptr<GameObject> LoadBrick(const std::string& row, glm::vec2& brickPos, int& x, int& y,
                                                 GamePlayState& gameState);
};
