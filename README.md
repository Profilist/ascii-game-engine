# ASCII Game Engine

An ASCII art game engine built from scratch in C++20, including two sample games (Flappy Bird and Space Invaders).

## Table of Contents
- [Features](#features)
- [Architecture](#architecture)
- [Class Documentation](#class-documentation)
  - [View](#view)
  - [Controller](#controller)
  - [Model](#model)
  - [World & Entity System](#world--entity-system)
  - [Event System](#event-system)
  - [Sound System](#sound-system)
- [Design Patterns](#design-patterns)
- [Example Games](#example-games)

## Features

- **MVC Architecture** 
    - Clean separation of concerns between game logic, rendering, and input
- **Efficient Console Rendering** 
    - Ncurses view with double buffers to minimize unnecessary re-renders
- **Event-Driven Architecture** 
    - Decoupled event system for game logic communication
- **Entity System** 
    - Flexible entity management with movement components and animations
- **Collision Detection** 
    - Hitbox-based collision system with z-layer support
- **Multi-layered Rendering** 
    - Z-order based rendering with height-based draw ordering
- **Resource Management** 
    - Centralized shape management with shared ownership
- **Modular Sound System** 
    - Abstract audio interface supporting multiple backends (SDL, Terminal)
- **RAII Memory Management** 
    - Zero manual memory management using smart pointers
- **C++20 Modules** 
    - Modern C++ module system for performance and modularity

## Architecture

The engine follows a strict MVC (Model-View-Controller) architecture to separate general engine responsibilities and promote low coupling and high cohesion. The `Engine` class acts as the concrete `Model` that coordinates all subsystems, while `CursesView` and `CursesController` provide the rendering and input handling respectively.

## Class Documentation

### View

The `View` is a pure abstract class inherited by `CursesView`. It defines a single virtual method `notify(drawables, statusLines)` that the model calls to render a frame. This design allows concrete views to provide their own rendering logic.

**CursesView Implementation:**
- Creates two windows in fixed layouts: one for the game and one for status lines
- Uses double-buffering with `scratchBuffer` (rebuilt each frame) and `prevBuffer` (previous frame) to render only rows that differ, minimizing unnecessary redraws
- Implements RAII: Ncurses `WINDOW*` instances are owned by `WinPtr` (unique_ptr with custom deleter), and Ncurses lifetime is managed inside `CursesView` (initscr() in constructor, endwin() in destructor)

### Controller

Like `View`, the `Controller` is a pure abstract class inherited by `CursesController`. It defines a single virtual method `getInput()` that the model calls to retrieve input events.

**CursesController Implementation:**
- Returns an `InputEvent` variant:
  - `NoInput{}` when no key is pressed
  - `KeyboardInput{key}` for key presses
- This design can be easily extended for other input types (e.g., mouse input)

### Model

The `Model` is an abstract class that provides base functionality for an MVC-based engine. It maintains a list of registered views and a controller pointer, providing the central coordination layer.

**Model Features:**
- Supports view registration via `addView()`, `removeView()`, and `notifyViews()`
- Supports multiple views observing the same model state (Observer pattern)
- Stores a single controller pointer attached via `setController()`
- Defines three pure virtual methods for subclasses:
  - `collectDrawables()` - provides render-specific data to the view
  - `collectStatus()` - provides status lines
  - `run()` - starts the main loop

**Engine** is the concrete implementation of `Model` that clients use. It owns and coordinates major subsystems:

- **Subsystems:** Clock, World, EventManager, ResourceManager, and SoundSystem
- **Game Loop:** Implements a consistent order for every tick:
  1. Get input from controller
  2. Run game-specific callback (via `setGameUpdate()`)
  3. Update world (movement, collisions, borders, etc.)
  4. Process events
  5. Notify views to render the frame
  6. Sleep to maintain constant refresh rate

**Clock** encapsulates timekeeping and frame rate control:
- Stores configurable tick duration (default 60 FPS)
- `sleepUntilNextTick()` sleeps for remaining time in current tick, preventing the game loop from running as fast as possible

### World & Entity System

**World** encapsulates the simulation state of the game:
- Owns the set of entities
- Enforces border rules (Solid or View mode)
- Detects collisions using hitbox intersection and z-layer checks
- Provides data needed for rendering entities and status information
- Updates all entities via `World::update(input)`, where each entity:
  - Gets its own `Entity::update(input)` called
  - Has collisions detected
  - Has border rules applied
  - Is removed if no longer alive
- Collects `Drawable`s for rendering and sorts by z-order
- Stores status lines for the view

**Entity** is the object abstraction:
- Contains identity (id, tag) and spatial state (position, hitbox)
- Has a base shape for rendering (optionally animated)
- Contains a set of movement components that determine behavior each tick
- `Entity::update(input)`:
  - Applies each `MovementComponent` to update position
  - Applies next animation frame (optional)
  - Increments ageTicks for despawn logic
- Provides `setOnCollision(callback)` for custom collision handling
- Stores `Solidity` (Solid, Trigger, Ghost) for collision detection

**MovementComponent** is a pure abstract class with a single virtual method `apply(entity, input)`:
- Decouples movement logic from Entity (improves reusability)
- Allows entities to hold multiple movement behaviors
- Concrete implementations:
  - `StraightMovement` - constant velocity in a straight-line path
  - `GravityMovement` - constant downward velocity
  - `CycleMovement` - periodic sequence of position offsets
  - `PlayerControlledMovement` - keyboard-based movement with configurable bindings

**Resources (Shape, Drawable, ResourceManager):**
- Rendering system is decoupled from view - model provides lightweight `Drawable`s
- `Shape` stores sprite id and 2D vector of characters, exposes `at(row, col)` for pixel access
- `Drawable` is lightweight class containing (shape*, x, y, z) for view rendering
- `ResourceManager` owns shapes (unique_ptr) and returns non-owning pointers for sharing

### Event System

The event system follows an Observer pattern:
- `EventManager` acts as central subject that components subscribe to
- Subscribers are notified via callbacks when events are dispatched
- Keeps subsystems decoupled while allowing game logic to react to events

**Built-in Events:**
- `CollisionEvent` - entity collisions
- `BorderEvent` - entity hitting world border
- `SoundEvent` - audio playback requests
- `GameOverEvent` - game state changes

**Event System Features:**
- Events inherit from common pure abstract base class with string `type()` discriminator
- `EventManager` supports `subscribe(type, callback)`, returning subscription ID
- Events are queued via `emit(...)` and dispatched in batches every tick via `processEvents()`
- Games can define custom event subclasses

### Sound System

Sound is implemented as a subsystem behind an abstract interface:
- Games request sounds via `SoundEvent`s through the event system
- Engine abstracts away audio backend logic
- Pure abstract class `SoundSystem` with virtual methods for playing, stopping, and mute control
- Engine holds `unique_ptr<SoundSystem>` for runtime backend swapping

**Supported Backends:**
- `TerminalSoundSystem` - Ncurses terminal beep() (default, always compatible)
- `SDLSoundSystem` - Real audio playback via SDL2_mixer
- `NullSoundSystem` - Silent backend for testing

**RAII Implementation:**
- `SoundClip` maintains RAII for SDL `Mix_Chunk*` by freeing audio resource in destructor
- Similar to how `CursesView` manages `WINDOW*` with `WinPtr`

## Design Patterns

### MVC
- Separation between Model/Engine (game state + orchestration), View/CursesView (rendering), and Controller/CursesController (user input)

### Observer Pattern
- Model maintains `vector<View*>` and notifies all registered views via `notifyViews()`
- EventManager maintains callback subscriptions: systems observe events by subscribing, world produces notifications by emitting events

### Strategy Pattern
- Movement behaviors are encapsulated as strategies through `MovementComponent::apply(...)` that can be swapped/combined per entity

### Command Pattern
- `InputEvent` is an extensible variant of input commands, allowing controller to return a single typed result that engine/world can interpret without depending on Ncurses details

### Factory Pattern
- `createSoundSystem(SoundBackend)` is a factory function that takes an enum value and returns the appropriate concrete `SoundSystem` implementation behind the abstract interface

### RAII (Resource Acquisition Is Initialization)
- All memory management handled via vectors and smart pointers
- External library resources (Ncurses `WINDOW*`, SDL `Mix_Chunk*`) wrapped with RAII
- Zero `delete` statements in the codebase

## Example Games

Two complete games are included to demonstrate the engine's capabilities:

### Flappy Bird
```bash
./age -g1
```

<img width="1548" height="957" alt="flappy" src="https://github.com/user-attachments/assets/ade949de-6edd-4b24-8764-69af497c7429" />


### Space Invaders
```bash
./age -g2
```

<img width="1540" height="951" alt="space invaders" src="https://github.com/user-attachments/assets/981109a5-4919-483f-80c9-1e6c80d32227" />


---

*Note: Due to academic integrity, the full source code is not provided. Please contact me at larris.xie@uwaterloo.ca for implementation details.*

