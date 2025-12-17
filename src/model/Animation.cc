export module entity.animation;

import <vector>;

import render.shape;

export namespace age {

struct Frame {
    const Shape* shape;
    int durationTicks;
    int offsetX;
    int offsetY;

    Frame() = default;
    Frame(const Shape* s, int duration, int offX = 0, int offY = 0);
};

class Animation {
public:
    Animation() = default;
    explicit Animation(std::vector<Frame> frames, bool looping = true);

    void advanceTick();

    void reset();

    const Shape* getCurrentShape() const;

    int getCurrentOffsetX() const;

    int getCurrentOffsetY() const;

    bool isFinished() const;

    bool empty() const;
    void setLooping(bool loop);
    bool isLooping() const;

private:
    std::vector<Frame> frames_;
    std::size_t currentIndex_;
    int tickCounter_;
    bool looping_;
};

}
