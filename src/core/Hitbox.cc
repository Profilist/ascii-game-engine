export module core.hitbox;

import core.position;

export namespace age {

class Hitbox {
public:
    Hitbox() = default;
    Hitbox(int offsetX, int offsetY, int width, int height);

    int offsetX() const noexcept;
    int offsetY() const noexcept;
    int width() const noexcept;
    int height() const noexcept;

    void setOffset(int x, int y) noexcept;

    void setSize(int w, int h) noexcept;

    // Check intersection with another hitbox given their world positions
    bool intersects(const Hitbox& other,
                    const Position& selfPos,
                    const Position& otherPos) const noexcept;

private:
    int offsetX_{0};
    int offsetY_{0};
    int width_{0};
    int height_{0};
};

}
