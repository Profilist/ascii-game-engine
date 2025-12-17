// Example game implementation demonstrating the engine API

#include <ncurses.h>

import <memory>;
import <string>;
import <vector>;
import <random>;

import audio.sound;
import controller;
import core.input_event;
import core.position;
import engine;
import entity;
import entity.animation;
import events.event;
import events.manager;
import render.shape;

import view;
import world;

namespace age {

enum class FlappyAction {
    None,
    Flap
};

class FlappyBirdGame {
public:
    void run() {
        // Create MVC components
        CursesView view;
        CursesController controller;
        Engine engine;
        engine.addView(&view);
        engine.setController(&controller);

        // Setup SDL sound system
        auto sdlSound = std::make_unique<SDLSoundSystem>();
        sdlSound->loadSound("flap", "assets/sounds/flappy_bird/flap.wav");
        sdlSound->loadSound("score", "assets/sounds/flappy_bird/score.wav");
        sdlSound->loadSound("die", "assets/sounds/flappy_bird/die.wav");
        engine.setSoundSystem(std::move(sdlSound));

        // Configure world
        World& world = engine.world();
        world.setBorderMode(World::BorderMode::Solid);

        // Setup game
        setupEventHandlers(engine, world);
        createBird(engine, world);

        // Register per-tick logic
        engine.setGameUpdate([this, &engine, &world](float dt, const InputEvent& input) {
            // Handle player input
            FlappyAction action = translateInput(input);
            if (action == FlappyAction::Flap) {
                handleFlap(engine);
            }

            // Implementation details removed but would spawn pipes periodically

            updateStatusLines(world);
        });

        engine.run();
    }

private:
    // Game state
    std::shared_ptr<Entity> bird_{nullptr};

    int nextEntityId_{1};
    int pipeSpawnTimer_{60};  // Start with first pipe at 60 ticks
    int score_{0};
    bool gameOver_{false};

    // Shapes (actual shapes would be defined here)
    Shape birdShape_{"bird", {/* shape definition */}};
    Shape birdFlapShape_{"bird_flap", {/* shape definition */}};
    std::vector<std::unique_ptr<Shape>> pipeShapes_;

    // Example: Setting up event handlers
    void setupEventHandlers(Engine& engine, World& world) {
        // Subscribe to collision events
        engine.events().subscribe("collision", [this, &engine, &world](const Event& e) {
            const auto& ce = static_cast<const CollisionEvent&>(e);
            if (ce.isBetween("bird", "pipe")) {
                triggerGameOver(engine, world);
            }
        });

        // Subscribe to border events
        engine.events().subscribe("border", [this, &engine, &world](const Event& e) {
            // Implementation details removed but would handle collisions with borders
        });
    }

    // Example: Creating entities with movement and animations
    void createBird(Engine& engine, World& world) {
        // Create bird entity
        bird_ = world.createEntity(nextEntityId_++, "bird", Position{BIRD_X, 10}, &birdShape_);
        bird_->addMovement(std::make_unique<GravityMovement>(FALL_SPEED));
        bird_->setSolidity(Solidity::Solid);

        // Set bird animation
        std::vector<Frame> frames;
        frames.push_back({&birdShape_, 25});
        frames.push_back({&birdFlapShape_, 15});
        bird_->setAnimation(std::make_unique<Animation>(std::move(frames), true));

        // Set collision callback
        bird_->setOnCollision([this, &engine, &world](Entity& self, Entity& other) {
            // Implementation details removed but would handle collisions with pipes and score triggers
        });

        world.setPlayer(bird_);
    }

    // Example: Spawning entities with movement components
    void spawnPipePair(World& world) {
        // Implementation details removed but shows the pattern:
        // 1. Calculate positions
        // 2. Create shapes
        // 3. Create entities with movement
        // 4. Set properties (solidity, clampToBorders, etc.)
        
        auto topPipe = world.createEntity(nextEntityId_++, "pipe", Position{/* ... */}, /* shape */);
        topPipe->addMovement(std::make_unique<StraightMovement>(PIPE_SPEED, 0.0f));
        topPipe->setSolidity(Solidity::Solid);
        topPipe->setClampToBorders(false);
        
        // Similar for bottom pipe and score trigger...
    }

    Shape* createScoreTriggerShape() {
        // Implementation details removed
        return nullptr;
    }

    Shape* createPipeShape(const std::string& name, int height, bool isTop) {
        // Implementation details removed
        return nullptr;
    }

    // Example: Input translation
    FlappyAction translateInput(const InputEvent& input) const {
        const KeyboardInput* kb = getKeyboardInput(input);
        if (!kb) return FlappyAction::None;

        switch (kb->key) {
            case ' ':
            case 'w':
            case 'W':
            case KEY_UP:
                return FlappyAction::Flap;
            default:
                return FlappyAction::None;
        }
    }

    // Example: Handling game actions
    void handleFlap(Engine& engine) {
        if (!bird_ || !bird_->isAlive() || gameOver_) return;

        bird_->move(0, -FLAP_HEIGHT);
        engine.events().emit<SoundEvent>("flap");
    }

    void triggerGameOver(Engine& engine, World& world) {
        if (gameOver_) return;
        
        gameOver_ = true;

        engine.events().emit<SoundEvent>("die");
        engine.events().emit<GameOverEvent>(false);

        updateStatusLines(world);
    }

    void updateStatusLines(World& world) {
        std::vector<std::string> lines;
        if (gameOver_) {
            lines.push_back("=== GAME OVER ===");
            lines.push_back("Final Score: " + std::to_string(score_));
            lines.push_back("Press 'q' to quit");
        } else {
            lines.push_back("FLAPPY BIRD | Score: " + std::to_string(score_));
            lines.push_back("Press SPACE or UP to flap");
            lines.push_back("Press 'q' to quit | 'm' to toggle mute");
        }
        world.setStatusLines(std::move(lines));
    }
};

    void runFlappyBird() {
        FlappyBirdGame game;
        game.run();
    }
}