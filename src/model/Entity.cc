export module entity;

import <functional>;
import <memory>;
import <string>;
import <vector>;

import core.hitbox;
import core.input_event;
import core.position;
import entity.animation;
import render.drawable;
import render.shape;

export namespace age {

// Solidity determines collision response behavior
enum class Solidity {
    Solid,    // Cannot pass through, fires collision event
    Trigger,  // Can pass through, fires collision event
    Ghost     // Can pass through, no collision event
};

class Entity;

// Collision callback type
using CollisionCallback = std::function<void(Entity& self, Entity& other)>;

// Abstract base for movement behaviors
class MovementComponent {
public:
    virtual ~MovementComponent() = default;
    virtual void apply(Entity& entity, const InputEvent& input) = 0;
};

// Constant velocity movement (velocity in pixels per tick)
class StraightMovement final : public MovementComponent {
public:
    StraightMovement(float vx, float vy);

    void apply(Entity& entity, const InputEvent& input) override;

    float velocityX() const;
    float velocityY() const;
    void setVelocity(float vx, float vy);

private:
    float velocityX_;
    float velocityY_;
    float accumulatorX_;
    float accumulatorY_;
};

// Cycles through position offsets
class CycleMovement final : public MovementComponent {
public:
    // interval is in ticks (e.g. 5 = change position every 5 ticks)
    CycleMovement(std::vector<Position> offsets, int intervalTicks);

    void apply(Entity& entity, const InputEvent& input) override;
    
    void reset();

private:
    std::vector<Position> offsets_;
    int currentIndex_;
    int intervalTicks_;
    int tickCounter_;
};

// Constant vertical velocity (for gravity/falling)
class GravityMovement final : public MovementComponent {
public:
    explicit GravityMovement(float fallSpeed);

    void apply(Entity& entity, const InputEvent& input) override;
    
    float fallSpeed() const;
    void setFallSpeed(float fallSpeed);

private:
    float fallSpeed_;
    float accumulator_;
};

// Keyboard-controlled movement
class PlayerControlledMovement final : public MovementComponent {
public:
    PlayerControlledMovement(float speed, int left, int right, int up, int down);

    void apply(Entity& entity, const InputEvent& input) override;
    
    void setMoveSpeed(float speed);

private:
    float moveSpeed_;
    int keyLeft_;
    int keyRight_;
    int keyUp_;
    int keyDown_;
};

// Game object
class Entity {
public:
    Entity(int id, std::string tag, Position pos, const Shape* shape);
    ~Entity() = default;

    // Non-copyable, but movable
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    // Core update method - applies movements and advances animation
    void update(const InputEvent& input);
    
    // Called by World when collision detected
    void onCollision(Entity& other);

    // Rendering
    Drawable toDrawable() const;

    // Lifecycle
    bool isAlive() const;
    void kill();

    // Getters
    int id() const;
    const std::string& tag() const;
    const Position& position() const;
    const Position& prevPosition() const;
    const Hitbox& hitbox() const;
    int height() const;
    Solidity solidity() const;
    const Shape* baseShape() const;
    int ageTicks() const;
    int maxAgeTicks() const;
    bool clampToBorders() const;
    Animation* animation();
    const Animation* animation() const;

    // Setters
    void setPosition(Position pos);
    void setPosition(int x, int y);
    void move(int dx, int dy);
    void setHitbox(Hitbox hb);
    void setHeight(int h);
    void setSolidity(Solidity s);
    void setBaseShape(const Shape* shape);
    void setMaxAgeTicks(int max);
    void setClampToBorders(bool clamp);
    void setAnimation(std::unique_ptr<Animation> anim);
    void setOnCollision(CollisionCallback cb);

    // Movement component management
    void addMovement(std::unique_ptr<MovementComponent> movement);
    void clearMovements();
    template<typename T> T* getMovement();

private:
    int id_;
    std::string tag_;
    Position position_;
    Position prevPosition_;
    Hitbox hitbox_;
    int height_;
    bool alive_;
    Solidity solidity_;

    const Shape* baseShape_;
    std::vector<std::unique_ptr<MovementComponent>> movements_;
    std::unique_ptr<Animation> animation_;
    CollisionCallback onCollisionCallback_;

    int ageTicks_;
    int maxAgeTicks_;
    bool clampToBorders_;
};

}
