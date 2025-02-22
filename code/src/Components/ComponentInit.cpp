
#include "ComponentInit.h"

#include "CollisionComponent.h"
#include "Component.h"
#include "DamageOnCollisionComponent.h"
#include "HealthComponent.h"
#include "ScoreOnCollisionComponent.h"
#include "ScoreOnDestructionComponent.h"
#include "SpawnGameObjectOnDestructionComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

const std::unordered_map<std::string, ComponentInternalInitFunc>& GetComponentInitFuncs()
{
    // clang-format off
   static const std::unordered_map<std::string, ComponentInternalInitFunc> sComponentTypesInitFuncs = {
        Component::InitData<SpriteComponent>(),          
        Component::InitData<HealthComponent>(),
        Component::InitData<TransformComponent>(),       
        Component::InitData<CollisionBoxComponent>(),
        //Component::InitData<CollisionCircleComponent>()
        Component::InitData<DamageOnCollisionComponent>(),
        Component::InitData<ScoreOnCollisionComponent>(),
        Component::InitData<ScoreOnDestructionComponent>(),
        Component::InitData<SpawnGameObjectOnDestructionComponent>()
    };
    // clang-format on

    return sComponentTypesInitFuncs;
}