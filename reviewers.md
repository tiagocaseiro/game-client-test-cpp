# Information for reviewers

For this exercise, I decided to turn the classes in the /Games folder into a composition of self-contained Components, also known as GameObjects.

A **GameObject** is any entity that's part of the gameplay experience (e.g., Brick, Paddle, Ball, Power-Up), but instead of relying on concrete classes to describe each in-game entity, these same enties were broken donw into small **Components** that hold the necessary information to give our GameObjects some sort of behavior.

So a GameObject is just a container of differents Components that, when used together, give a GameObjects its characteristics.

## GameObject Implementation
A GameObject simply consist of a container of Components.

### Example
A normal Brick GameObject would contain a HealthComponent which would be responsible for holding the information regarding how many hits points it has left. 

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

This solution was developed and tested only on Windows 11.