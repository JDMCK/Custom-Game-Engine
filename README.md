# Custom Game Engine

Polygon Pit | Platformer Demo
:-:|:-:
![game-engine02](https://github.com/user-attachments/assets/1d9e3bf0-55a9-47df-abdf-583274de176b) | ![game-engine01](https://github.com/user-attachments/assets/a3e2a0b3-243e-4add-b158-b198452cdbdf)

## Technology
This custom game engine was made with SFML to support graphics and user input. The rest of the engine was built from scratch with c++ using an Entity Component System (ECS) architecture.

## Entity Component System
Entities are created with only an id initially, then components are programmatically attached to the entity to make it unique. Examples of components would be a health, movement, or cooldown component. At it's core, a component is just some state that marks the entity to have a unique attribute and the data that supports that attribute. Systems are the functions that will use the data within the component to apply the rules necessary for each component. For example, a system that is working on an entity with a cooldown component, might check to see if some timer is expired and then do something, or it might tick down the timer. The main benefit of an ECS engine is that each system is entity agnostic; they only care about working on a specific component or set of components. This automatically avoids many issues that a more object oriented approach would introduce.

## Games
Polygon Pit is a simple game with a player (red hexagon) and many randomly generated enemies. This game has some basic collision features as well as bullet shooting, lives, scoring, and power up mechanics. The goal is to get a high score befor your lives run out! The platformer demo was made to test some more advanced features of the engine such as rigid body physics, collisions, sprites, binded inputs, and animations.


This Entity Component System (ECS) game engine is my implementation of the game engine architecture covered in David Churchill's YouTube video series.
https://www.youtube.com/@DaveChurchill
