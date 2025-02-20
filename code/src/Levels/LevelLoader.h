// (C) king.com Ltd 2021
#pragma once

#include "Level.h"

#include <string>

class LevelLoader
{
public:
    static std::unique_ptr<Level> LoadLevel(const std::string& levelName, King::Engine& engine,
                                            Level::ScoreReportingFunction scoreReportingFunction);
};
