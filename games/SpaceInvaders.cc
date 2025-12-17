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

enum class SpaceAction {
    None,
    MoveUp,
    MoveDown,
    Shoot
};

class SpaceInvadersGame {
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
        sdlSound->loadSound("shoot", "assets/sounds/space_invaders/shoot.wav");
        sdlSound->loadSound("hit", "assets/sounds/space_invaders/hit.wav");
        sdlSound->loadSound("die", "assets/sounds/space_invaders/die.wav");
        sdlSound->loadSound("win", "assets/sounds/space_invaders/win.wav");
        engine.setSoundSystem(std::move(sdlSound));

        // Configure world
        World& world = engine.world();
        world.setBorderMode(World::BorderMode::Solid);

        // Setup game
        setupEventHandlers(engine, world);
        setupLevel(world, 1);

        // Register per-tick logic
        engine.setGameUpdate([this, &engine, &world](float dt, const InputEvent& input) {
            // Handle player input
            SpaceAction action = translateInput(input);
            handleAction(action, engine, world);

            updateEnemyMovement(world);
            updateEnemyShooting(world);

            // Check for level completion
            checkLevelComplete(engine, world);

            // Check for game over (enemy reached player zone)
            checkEnemyReachedPlayer(engine, world);

            updateStatusLines(world);
        });

        engine.run();
    }

private:
    // Game state
    std::shared_ptr<Entity> player_{nullptr};

    int nextEntityId_{1};
    int level_{1};
    int score_{0};
    int enemiesRemaining_{0};
    bool gameOver_{false};
    bool victory_{false};

    // Player shooting cooldown
    int shootCooldown_{0};

    // Enemy movement state
    float enemyMoveAccumulator_{0.0f};
    int enemyDirection_{1};  // 1 = down, -1 = up
    int enemyShootTimer_{60};

    // Shapes (actual shapes would be defined here)
    Shape playerShapeA_{"player_a", {/* shape definition */}};
    Shape playerShapeB_{"player_b", {/* shape definition */}};
    Shape bulletShape_{"bullet", {/* shape definition */}};
    Shape enemyBulletShape_{"enemy_bullet", {/* shape definition */}};
    Shape enemyShapeA_{"enemy_a", {/* shape definition */}};
    Shape enemyShapeB_{"enemy_b", {/* shape definition */}};
    Shape starShapeA_{"star_a", {/* shape definition */}};
    Shape starShapeB_{"star_b", {/* shape definition */}};

    // Example: Setting up event handlers
    void setupEventHandlers(Engine& engine, World& world) {
        // Subscribe to collision events
        engine.events().subscribe("collision", [this, &engine, &world](const Event& e) {
            // Implementation details removed but would handle:
            // - Bullet vs bullet collisions
            // - Player bullet hits enemy
            // - Enemy bullet hits player
            // - Enemy touches player directly
        });
    }

    void setupLevel(World& world, int level) {
        level_ = level;
        
        // Clear existing entities
        for (auto& entity : world.entities()) {
            if (entity) entity->kill();
        }
        
        // Reset state
        enemyMoveAccumulator_ = 0.0f;
        enemyDirection_ = 1;
        shootCooldown_ = 0;
        
        const LevelConfig& config = LEVELS[level-1];
        enemyShootTimer_ = config.shootInterval;

        // Create player
        createPlayer(world);

        // Background decorations
        spawnStars(world, 8);

        // Create enemy grid
        spawnEnemyGrid(world, config.enemyRows, config.enemyCols);
    }

    // Example: Creating entities with animations
    void createPlayer(World& world) {
        int startY = world.height()/2 - 1;
        player_ = world.createEntity(nextEntityId_++, "player", Position{PLAYER_X, startY}, &playerShapeA_);
        player_->setSolidity(Solidity::Solid);
        player_->setClampToBorders(true);
        world.setPlayer(player_);

        std::vector<Frame> frames;
        frames.push_back({&playerShapeA_, ANIM_INTERVAL_TICKS});
        frames.push_back({&playerShapeB_, ANIM_INTERVAL_TICKS});
        player_->setAnimation(std::make_unique<Animation>(std::move(frames), true));
    }

    // Example: Spawning multiple entities
    void spawnEnemyGrid(World& world, int rows, int cols) {
        enemiesRemaining_ = rows*cols;
        
        // Implementation details removed but would:
        // 1. Calculate grid positions
        // 2. Create enemy entities in a grid pattern
        // 3. Set up animations for each enemy
    }

    void spawnStars(World& world, int count) {
        // Implementation details removed but would spawn animated background star entities
    }

    // Example: Input translation
    SpaceAction translateInput(const InputEvent& input) const {
        const KeyboardInput* kb = getKeyboardInput(input);
        if (!kb) return SpaceAction::None;

        switch (kb->key) {
            case 'w':
            case 'W':
            case KEY_UP:
                return SpaceAction::MoveUp;
            case 's':
            case 'S':
            case KEY_DOWN:
                return SpaceAction::MoveDown;
            case ' ':
                return SpaceAction::Shoot;
            default:
                return SpaceAction::None;
        }
    }

    // Example: Handling game actions
    void handleAction(SpaceAction action, Engine& engine, World& world) {
        if (!player_ || !player_->isAlive()) return;

        switch (action) {
            case SpaceAction::MoveUp:
                player_->move(0, -PLAYER_SPEED);
                break;
            case SpaceAction::MoveDown:
                player_->move(0, PLAYER_SPEED);
                break;
            case SpaceAction::Shoot:
                shootPlayerBullet(engine, world);
                break;
            default:
                break;
        }
    }

    // Example: Creating entities with movement components
    void shootPlayerBullet(Engine& engine, World& world) {
        if (shootCooldown_ > 0) return;
        
        // Spawn bullet at player's right side
        int bulletX = player_->position().x + 4;
        int bulletY = player_->position().y + 1;
        
        auto bullet = world.createEntity(nextEntityId_++, "player_bullet", Position{bulletX, bulletY}, &bulletShape_);
        bullet->addMovement(std::make_unique<StraightMovement>(BULLET_SPEED, 0.0f));
        bullet->setSolidity(Solidity::Trigger);
        bullet->setClampToBorders(false);
        
        shootCooldown_ = SHOOT_COOLDOWN;
        engine.events().emit<SoundEvent>("shoot");
    }

    void updateEnemyMovement(World& world) {
        // Implementation details removed but would:
        // 1. Accumulate movement over time
        // 2. Check for boundary collisions
        // 3. Move all enemies and reverse direction when hitting edges
    }

    void updateEnemyShooting(World& world) {
        // Implementation details removed but would:
        // 1. Decrement shooting timer
        // 2. Select random enemy to shoot
        // 3. Spawn enemy bullet with movement component
    }

    void checkLevelComplete(Engine& engine, World& world) {
        if (enemiesRemaining_ <= 0) {
            if (level_ < 2) {
                // Advance to next level
                level_++;
                setupLevel(world, level_);
                engine.events().emit<SoundEvent>("win");
            } else {
                // Victory
                victory_ = true;
                engine.events().emit<SoundEvent>("win");
                engine.events().emit<GameOverEvent>(true);
            }
        }
    }

    void checkEnemyReachedPlayer(Engine& engine, World& world) {
        // Implementation details removed but would check if any enemy reached danger zone
    }

    void triggerGameOver(Engine& engine) {
        if (gameOver_) return;
        
        gameOver_ = true;

        engine.events().emit<SoundEvent>("die");
        engine.events().emit<GameOverEvent>(false);
    }

    void updateStatusLines(World& world) {
        std::vector<std::string> lines;

        if (victory_) {
            lines.push_back("=== VICTORY! ===");
            lines.push_back("Final Score: " + std::to_string(score_));
            lines.push_back("You defeated all invaders!");
            lines.push_back("Press 'q' to quit");
        } else if (gameOver_) {
            lines.push_back("=== GAME OVER ===");
            lines.push_back("Final Score: " + std::to_string(score_));
            lines.push_back("The invaders have won...");
            lines.push_back("Press 'q' to quit");
        } else {
            lines.push_back("SPACE INVADERS | Level: " + std::to_string(level_) + 
                          " | Score: " + std::to_string(score_) +
                          " | Enemies: " + std::to_string(enemiesRemaining_));
            lines.push_back("W/S or UP/DOWN to move | SPACE to shoot");
            lines.push_back("Press 'q' to quit | 'm' to toggle mute");
        }

        world.setStatusLines(std::move(lines));
    }
};

    void runSpaceInvaders() {
        SpaceInvadersGame game;
        game.run();
    }
}
