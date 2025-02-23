
#include "ComponentInit.h"

#include "CollisionComponent.h"
#include "Component.h"
#include "DamageOnCollisionComponent.h"
#include "DestroyOnCollisionComponent.h"
#include "HealthComponent.h"
#include "RigidBodyComponent.h"
#include "ScoreOnCollisionComponent.h"
#include "ScoreOnDestructionComponent.h"
#include "SetPaddleDataComponent.h"
#include "SpawnGameObjectOnDestructionComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

const std::unordered_map<std::string, ComponentInitFunc>& GetComponentInitFuncs()
{
    // clang-format off
   static const std::unordered_map<std::string, ComponentInitFunc> sComponentTypesInitFuncs = {
        Component::InitData<SpriteComponent>(),          
        Component::InitData<HealthComponent>(),
        Component::InitData<TransformComponent>(),       
        Component::InitData<CollisionBoxComponent>(),
        Component::InitData<CollisionCircleComponent>(),
        Component::InitData<DamageOnCollisionComponent>(),
        Component::InitData<ScoreOnCollisionComponent>(),
        Component::InitData<ScoreOnDestructionComponent>(),
        Component::InitData<SpawnGameObjectOnDestructionComponent>(),
        Component::InitData<RigidBodyComponent>(),
        Component::InitData<DestroyOnCollisionComponent>(),
        Component::InitData<SetPaddleDataComponent>(),
     };
    // clang-format on

    return sComponentTypesInitFuncs;
}