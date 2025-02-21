// (C) king.com Ltd 2021
#pragma once

#include "Level.h"

#include <string>

#include "Components/Component.h"

class LevelLoader
{
public:
    static std::unique_ptr<Level> LoadLevel(const std::string& levelName, King::Engine& engine,
                                            const ComponentsInitData&,
                                            Level::ScoreReportingFunction scoreReportingFunction);
};
