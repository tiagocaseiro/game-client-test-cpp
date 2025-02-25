
#include "ComponentInit.h"

#include "CollisionComponent.h"
#include "Component.h"
#include "DamageOnCollisionComponent.h"
#include "DestroyOnCollisionComponent.h"
#include "DestroyOnGameStateReset.h"
#include "HealthComponent.h"
#include "PowerUpComponent.h"
#include "RigidBodyComponent.h"
#include "ScoreOnCollisionComponent.h"
#include "ScoreOnDestructionComponent.h"
#include "SpawnGameObjectOnDestructionComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

const std::unordered_map<std::string, ComponentInitFunc>& GetComponentInitFuncs()
{
    // clang-format off
    // Map that contains the Name of the Component and the initialization function that is able to create a new instance from the serialized data
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
        Component::InitData<PaddleResizePowerUpComponent>(),
        Component::InitData<DestroyOnGameStateReset>(),
     };
    // clang-format on

    return sComponentTypesInitFuncs;
}