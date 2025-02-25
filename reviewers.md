# Information for reviewers

For this exercise, the approach was to turn each class in the /Games folder into a composition of self-contained Components, also known as GameObjects.
It's an ECS-like approach of breaking classes into small and reusable Components.

Unlike pure ECS, the components for this exercise don't just hold data but are able to execute their own behavior.

A **GameObject** is any entity that's part of the gameplay experience (e.g., Brick, Paddle, Ball, Power-Up), but instead of relying on concrete classes to describe each in-game entity, these same enties were broken donw into small **Components** that hold the necessary information to give our GameObjects some sort of behavior.

So a GameObject is just a container of different Components that, when used together, give a GameObject its characteristics.

## GameObject Implementation
A GameObject simply consists of a container of Components.

### Example
A normal Brick GameObject would contain a HealthComponent which would be responsible for holding the information regarding how many hit points it has left. 

The HealthComponent would mark its GameObject for destruction when its health ticks all the way down to 0.

## Component Implementation
Every Component that composes a GameObject is derived from the abstract Component class that has the following virtual member functions:
- void OnCreate()
    - Called when GameObject's initial set of Components is initialized
- void Render();
    - Called every GameState Render call
- void Update();
    - Called every GameState Update call
- void OnDestroyed();
    - Called right before destructor is called
- void OnComponentAdded(std::shared_ptr\<Component\>);
    - Called when a new sibling component is Added to GameObject
    
### Component Catalog
- TransformComponent
    - Holds transform data (position, rotation, scale) 
- SpriteComponent
    - Holds textures that can be rendered
    - In charge of rendering one of the textures
    - Uses information from TransformComponent for rendering
- HealthComponent
    - Holds hit point data
    - Marks GameObject for destruction when hit points tick down to 0
    - Depending on its health data, tells SpriteComponent which texture to render
- CollisionBoxComponent
    - Defines a collision box 
- RigidBodyComponent
    - Defines a rigid body
- CollisionCircleComponent
    - Defines a collision circle
- DamageOnCollisionComponent
    - Ticks HealthComponent down when the collider provider by one of the CollisionComponents detects a collision
- DestroyOnCollisionComponent
    - Immediately marks GameObject for destruction down when the collider provider by one of the CollisionComponents detects a collision
- ScoreOnCollisionComponent
    - Updates score when collider provider by one of the CollisionComponents detects a collision
- ScoreOnDestructionComponent
    - Updates score when object is destroyed
- SpawnGameObjectOnDestructionComponent
    - Spawn a new GameObject after destruction
- PaddleResizePowerUpComponent
    - Is able to temporarily change the Paddle's size and sprite when the collider provider by one of the CollisionComponents detects a collision
- DestroyOnGameStateReset
    - Mark GameObject for destruction whenever GameState is reset (done by GameState)

## Serialization
In an effort to data-drive this solution, each GameObject's definition, also known as **GameObjectTemplate**, is stored in a .txt file that describes the Components that need to be instantiated and the necessary parameters with which they should be initialized.
These files can be found in the code/assets/GameObjectTemplates folder.

### Example 
The base Brick GameObjectTemplate has the following configuration in the BrickGameObjectTemplates.txt file:
```
id=BaseBrickGameObject

componentStart=SpriteComponent
sprite1=Brick-red.png
componentEnd=

componentStart=TransformComponent
scale=2
componentEnd=

componentStart=CollisionBoxComponent
width=64
height=32
layer=2
mask=0
componentEnd=

componentStart=ScoreOnDestructionComponent
score=100
componentEnd=
```

## Inheritance
In order to prevent these files from being flooded with repeated Component entries, it's possible define dependencies from previous configurations by adding a *parent=ExampleGameObject* entry to its configuration. 

### Example
The configuration for the grey bricks has a *parent=BaseBrickGameObject* entry which will inject all of BaseBrickGameObject's Components into every grey brick
```
id=c

parent=BaseBrickGameObject

componentStart=SpriteComponent
sprite1=Brick-grey.png
sprite2=Brick-grey-broken.png
componentEnd=

componentStart=HealthComponent
maxHealth=2
componentEnd=

componentStart=DamageOnCollisionComponent
componentEnd=

componentStart=ScoreOnDestructionComponent
score=200
componentEnd=

componentStart=ScoreOnCollisionComponent
score=10
componentEnd=
```

### GameObjectTemplate Implementation
In code, a GameObjectTemplate is just a map that holds pairs of GameObjectTemplate Id's (e.g., PaddleGameObject) and all of the necessary functions to initialize each Component that was serialized.

## Limitations
The initial goal of this side project was to break all the main gameplay concepts into GameObjects but due to time constraints and risk mitigation only Bricks and Power-Ups were covered by this change.
All Brick definitions can be found in the BrickGameObjectTemplates.txt file and PowerUps can be found in the PowerUpGameObjectTemplates.txt file.

Out of convenience, GameObjectTemplate ids for Bricks match the tile entries on the LevelN.txt files, instead of following the \<Name\>GameObject format (e.g., p instead of PinkBrickGameObject).

This solution was developed and tested only on Windows 11.

## Improvements
The one PowerUpComponent that was created is way too bespoke and too specific towards the paddle. Transitioning the Paddle into this GameObject framework would allow the flexibility to not have this Component as bespoke. There’s the possibility of having its ownership being transferred from the PowerUpGameObject to the PaddleGameObject where is Update function would execute its PoweUp effect.

Components would benefit from having some of their behavior be managed by outside systems that are able to aggregate them and manipulate them for a specific tasks.

## Debug commands
Press 1 key to hit one brick.

Hold down 2 key to continuously hit bricks.