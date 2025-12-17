export module events.event;

import <memory>;
import <string>;

export namespace age {

// Abstract class for all game events
class Event {
public:
    virtual ~Event() = default;
    virtual const char* type() const noexcept = 0;
};

// Concrete event for entity collisions
class CollisionEvent final : public Event {
public:
    CollisionEvent(int entityA, int entityB, const std::string& tagA, const std::string& tagB);

    const char* type() const noexcept override;

    int entityAId() const;
    int entityBId() const;
    const std::string& tagA() const;
    const std::string& tagB() const;

    // Check if collision involves a specific tag
    bool involves(const std::string& tag) const;

    // Check if collision is between two specific tags
    bool isBetween(const std::string& tag1, const std::string& tag2) const;

private:
    int entityAId_;
    int entityBId_;
    std::string tagA_;
    std::string tagB_;
};

// Concrete event for game over
class GameOverEvent final : public Event {
public:
    explicit GameOverEvent(bool won, const std::string& reason = "");

    const char* type() const noexcept override;

    bool won() const;
    const std::string& reason() const;

private:
    bool won_;
    std::string reason_;
};

// Concrete event for playing a sound
class SoundEvent final : public Event {
public:
    explicit SoundEvent(const std::string& soundId);

    const char* type() const noexcept override;

    const std::string& soundId() const;

private:
    std::string soundId_;
};

// Concrete event for entity hitting the world border
class BorderEvent final : public Event {
public:
    enum class Side { Left, Right, Top, Bottom };

    BorderEvent(int entityId, Side side);

    const char* type() const noexcept override;

    int entityId() const;
    Side side() const;

private:
    int entityId_;
    Side side_;
};

}
