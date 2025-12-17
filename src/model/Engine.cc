export module engine;

import <functional>;
import <memory>;
import <string>;
import <vector>;

import audio.sound;
import controller;
import core.clock;
import core.input_event;
import events.event;
import events.manager;
import model;
import render.drawable;
import resources.manager;
import view;
import world;

export namespace age {

// Callback type for game-specific per-tick logic
// Called each frame with delta time and current input
using GameUpdateCallback = std::function<void(float dt, const InputEvent& input)>;

// Engine is a concrete Model (MVC)
// Owns and coordinates all game subsystems
class Engine : public Model {
public:
    Engine();

    ~Engine() override = default;

    // Model interface implementation
    std::vector<Drawable> collectDrawables() override;

    std::vector<std::string> collectStatus() override;

    // Main game loop
    void run() override;

    // Getters for game state
    bool isQuit() const noexcept;
    bool isGameOver() const noexcept;
    bool isWon() const noexcept;
    int level() const noexcept;
    int score() const noexcept;

    // Getters for subsystems
    World& world() noexcept;
    const World& world() const noexcept;
    EventManager& events() noexcept;
    ResourceManager& resources() noexcept;
    SoundSystem& sound() noexcept;

    // Setters for game state
    void setQuit(bool q) noexcept;
    void setGameOver(bool go) noexcept;
    void setLevel(int lvl) noexcept;
    void setScore(int s) noexcept;
    void addScore(int delta) noexcept;
    void setRefreshRate(int rate);
    void setSoundSystem(std::unique_ptr<SoundSystem> sound);

    // Set game-specific per-tick update callback
    // This is called each frame before world.update()
    void setGameUpdate(GameUpdateCallback callback);

private:
    // Game state
    bool quit_{false};
    bool gameOver_{false};
    bool won_{false};
    bool running_{false};
    int level_{1};
    int score_{0};
    int refreshRate_{60};

    // Subsystems (owned by Engine)
    Clock clock_;
    World world_;
    EventManager events_;
    ResourceManager resources_;
    std::unique_ptr<SoundSystem> sound_;

    // Game-specific callback (called each tick)
    GameUpdateCallback gameUpdate_;
};

}
