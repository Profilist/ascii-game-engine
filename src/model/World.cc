export module world;

import <algorithm>;
import <memory>;
import <string>;
import <vector>;

import core.hitbox;
import core.input_event;
import core.position;
import entity;
import events.event;
import events.manager;
import render.drawable;
import render.shape;

export namespace age {

class World {
public:
    enum class BorderMode {
        Solid,
        View
    };

    World(int width = 78, int height = 20, BorderMode borderMode = BorderMode::Solid);

    void update(const InputEvent& input);

    void handleCollisions();

    void applyBorderRules(Entity& entity);

    // Entity management
    void addEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> createEntity(int id, const std::string& tag, Position pos, const Shape* shape);
    void removeDeadEntities();

    // Collect drawables and status lines for rendering
    void collectDrawables(std::vector<Drawable>& out) const;
    void collectStatusLines(std::vector<std::string>& out) const;
    void clearStatusLines();
    void addStatusLine(const std::string& line);
    void setStatusLines(std::vector<std::string> lines);

    // Getters
    const Hitbox& border() const noexcept;
    BorderMode borderMode() const noexcept;
    int width() const noexcept;
    int height() const noexcept;
    int tickCount() const noexcept;
    const std::vector<std::shared_ptr<Entity>>& entities() const;
    std::shared_ptr<Entity> player() const;
    // Find entity by ID
    std::shared_ptr<Entity> findEntity(int id) const;
    // Find entities by tag
    std::vector<std::shared_ptr<Entity>> findEntitiesByTag(const std::string& tag) const;

    // Setters
    void setEventManager(EventManager* events);
    void setBorderMode(BorderMode mode);
    void setPlayer(std::shared_ptr<Entity> p);

private:
    Hitbox border_;
    BorderMode borderMode_;
    int width_;
    int height_;
    int tickCount_{0};
    std::vector<std::string> statusLines_;
    std::vector<std::shared_ptr<Entity>> entities_;
    std::shared_ptr<Entity> player_;
    EventManager* events_{nullptr}; // (owned externally by Engine)
};

}
