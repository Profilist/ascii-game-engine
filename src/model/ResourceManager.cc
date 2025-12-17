export module resources.manager;

import <memory>;
import <string>;
import <vector>;

import render.shape;

export namespace age {

// Centralized manager for game resources (shapes, sound clips, etc.)
class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Register a shape and return a pointer to it
    // The ResourceManager owns the shape memory
    const Shape* registerShape(std::string id, std::vector<std::string> pixels);

    // Get a shape by ID (returns nullptr if not found)
    const Shape* getShape(const std::string& id) const;

    // Check if a shape exists
    bool hasShape(const std::string& id) const;

    // Get all registered shape IDs
    std::vector<std::string> getShapeIds() const;

    // Clear all shapes
    void clear();

    // Get count of registered shapes
    size_t shapeCount() const;

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

}
