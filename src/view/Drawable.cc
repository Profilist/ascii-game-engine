export module render.drawable;

import <vector>;

import render.shape;

export namespace age {

class Drawable {
public:
    Drawable() = default;
    Drawable(const Shape* shape, int x, int y, int z = 0);

    const Shape* shape() const noexcept;
    int x() const noexcept;
    int y() const noexcept;
    int z() const noexcept;

    void setPosition(int x, int y) noexcept;

    void setZ(int z) noexcept;

private:
    const Shape* shape_{nullptr};
    int x_{0};
    int y_{0};
    int z_{0};
};

}
