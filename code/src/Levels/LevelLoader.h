// (C) king.com Ltd 2021
#pragma once

#include "Level.h"

#include <string>

#include "Components/Component.h"

class LevelLoader
{
public:
    static std::unique_ptr<Level> LoadLevel(const std::string& levelName, King::Engine& engine,
                                            Level::ScoreReportingFunction scoreReportingFunction);

private:
    static void LoadBrick(const std::string& row, Level& level, glm::vec2& brickPos, int& x, int& y);
};
